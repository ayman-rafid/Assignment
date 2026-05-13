#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

#include <msclr/marshal_cppstd.h>

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <windows.h>

#include "FileManager.h"
#include "Person.h"
#include "Processor.h"
#include "Types.h"

using namespace System;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Windows::Forms;

namespace {
std::string fromManagedString(String^ value) {
    String^ text = value == nullptr ? String::Empty : value;
    return msclr::interop::marshal_as<std::string>(text);
}

String^ toManagedString(const std::string& value) {
    return gcnew String(value.c_str());
}
}

public ref class GradeCalcForm : public Form {
public:
    GradeCalcForm() {
        Text = "Student Grade Calculator";
        StartPosition = FormStartPosition::CenterScreen;
        MinimumSize = Drawing::Size(940, 620);
        Size = Drawing::Size(1040, 700);

        currentSortField_ = SortField::Surname;

        buildMenu();
        buildLayout();
        seedExampleInput();
    }

private:
    MenuStrip^ menuStrip_;
    TextBox^ inputTextBox_;
    TextBox^ resultTextBox_;
    StatusStrip^ statusStrip_;
    ToolStripStatusLabel^ statusLabel_;
    SortField currentSortField_;

    void buildMenu() {
        menuStrip_ = gcnew MenuStrip();

        ToolStripMenuItem^ fileMenu = gcnew ToolStripMenuItem("&File");
        fileMenu->DropDownItems->Add("Open student file...", nullptr, gcnew EventHandler(this, &GradeCalcForm::openFile));
        fileMenu->DropDownItems->Add("Save student data...", nullptr, gcnew EventHandler(this, &GradeCalcForm::saveStudentData));
        fileMenu->DropDownItems->Add("Save results as...", nullptr, gcnew EventHandler(this, &GradeCalcForm::saveResults));
        fileMenu->DropDownItems->Add(gcnew ToolStripSeparator());
        fileMenu->DropDownItems->Add("Exit", nullptr, gcnew EventHandler(this, &GradeCalcForm::exitApplication));

        ToolStripMenuItem^ calculateMenu = gcnew ToolStripMenuItem("&Calculation");
        calculateMenu->DropDownItems->Add("Calculate final grade by average", nullptr, gcnew EventHandler(this, &GradeCalcForm::calculateAverage));
        calculateMenu->DropDownItems->Add("Calculate final grade by median", nullptr, gcnew EventHandler(this, &GradeCalcForm::calculateMedian));
        calculateMenu->DropDownItems->Add("Show average and median table", nullptr, gcnew EventHandler(this, &GradeCalcForm::calculateBoth));
        calculateMenu->DropDownItems->Add(gcnew ToolStripSeparator());
        calculateMenu->DropDownItems->Add("Sort by name", nullptr, gcnew EventHandler(this, &GradeCalcForm::sortByName));
        calculateMenu->DropDownItems->Add("Sort by surname", nullptr, gcnew EventHandler(this, &GradeCalcForm::sortBySurname));

        ToolStripMenuItem^ helpMenu = gcnew ToolStripMenuItem("&Help");
        helpMenu->DropDownItems->Add("About", nullptr, gcnew EventHandler(this, &GradeCalcForm::showAbout));

        menuStrip_->Items->Add(fileMenu);
        menuStrip_->Items->Add(calculateMenu);
        menuStrip_->Items->Add(helpMenu);
        MainMenuStrip = menuStrip_;
        Controls->Add(menuStrip_);
    }

    void buildLayout() {
        TableLayoutPanel^ root = gcnew TableLayoutPanel();
        root->Dock = DockStyle::Fill;
        root->ColumnCount = 1;
        root->RowCount = 3;
        root->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 30.0f));
        root->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 100.0f));
        root->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 24.0f));

        SplitContainer^ split = gcnew SplitContainer();
        split->Dock = DockStyle::Fill;
        split->Orientation = Orientation::Vertical;
        split->SplitterDistance = 500;

        inputTextBox_ = createTextBox(false);
        resultTextBox_ = createTextBox(true);

        split->Panel1->Controls->Add(createPanel("Student data", inputTextBox_));
        split->Panel2->Controls->Add(createPanel("Results", resultTextBox_));

        statusStrip_ = gcnew StatusStrip();
        statusLabel_ = gcnew ToolStripStatusLabel("Ready");
        statusStrip_->Items->Add(statusLabel_);

        root->Controls->Add(gcnew Panel(), 0, 0);
        root->Controls->Add(split, 0, 1);
        root->Controls->Add(statusStrip_, 0, 2);
        Controls->Add(root);
        menuStrip_->BringToFront();
    }

    TextBox^ createTextBox(bool readOnly) {
        TextBox^ textBox = gcnew TextBox();
        textBox->Dock = DockStyle::Fill;
        textBox->Multiline = true;
        textBox->ScrollBars = ScrollBars::Both;
        textBox->AcceptsReturn = true;
        textBox->AcceptsTab = true;
        textBox->WordWrap = false;
        textBox->ReadOnly = readOnly;
        textBox->Font = gcnew Drawing::Font("Consolas", 10.0f);
        return textBox;
    }

    Panel^ createPanel(String^ title, Control^ content) {
        Panel^ panel = gcnew Panel();
        panel->Dock = DockStyle::Fill;
        panel->Padding = System::Windows::Forms::Padding(10);

        Label^ label = gcnew Label();
        label->Text = title;
        label->Dock = DockStyle::Top;
        label->Height = 24;
        label->Font = gcnew Drawing::Font(label->Font, FontStyle::Bold);

        panel->Controls->Add(content);
        panel->Controls->Add(label);
        content->BringToFront();
        return panel;
    }

    void seedExampleInput() {
        inputTextBox_->Text =
            "Name Surname HW1 HW2 HW3 Exam\r\n"
            "Jonas Jonaitis 8 9 10 9\r\n"
            "Ona Onaite 4 5 6 5\r\n";
    }

    std::vector<Person> readStudentsFromInput() {
        std::istringstream input(fromManagedString(inputTextBox_->Text));
        std::vector<Person> students = loadStudentsFromStream(input, "student data textbox");

        if (students.empty()) {
            throw std::runtime_error("No valid student records were loaded.");
        }

        sortStudents(students, currentSortField_);
        return students;
    }

    void showError(const std::exception& ex) {
        MessageBox::Show(toManagedString(ex.what()), "Student Grade Calculator",
                         MessageBoxButtons::OK, MessageBoxIcon::Error);
        statusLabel_->Text = "Action failed";
    }

    void showResults(CalculationMethod method) {
        try {
            std::vector<Person> students = readStudentsFromInput();
            for (Person& student : students) {
                student.calculateFinalGrade(method);
            }

            resultTextBox_->Text = toManagedString(formatSingleMethodTable(students, method));
            statusLabel_->Text = method == CalculationMethod::Average
                ? "Calculated final grades by average"
                : "Calculated final grades by median";
        }
        catch (const std::exception& ex) {
            showError(ex);
        }
    }

    void calculateAverage(Object^, EventArgs^) {
        showResults(CalculationMethod::Average);
    }

    void calculateMedian(Object^, EventArgs^) {
        showResults(CalculationMethod::Median);
    }

    void calculateBoth(Object^, EventArgs^) {
        try {
            std::vector<Person> students = readStudentsFromInput();
            resultTextBox_->Text = toManagedString(formatAvgMedTable(students));
            statusLabel_->Text = "Calculated average and median table";
        }
        catch (const std::exception& ex) {
            showError(ex);
        }
    }

    void sortByName(Object^, EventArgs^) {
        currentSortField_ = SortField::Name;
        statusLabel_->Text = "Sorting by name";
        calculateBoth(nullptr, nullptr);
    }

    void sortBySurname(Object^, EventArgs^) {
        currentSortField_ = SortField::Surname;
        statusLabel_->Text = "Sorting by surname";
        calculateBoth(nullptr, nullptr);
    }

    void openFile(Object^, EventArgs^) {
        OpenFileDialog^ dialog = gcnew OpenFileDialog();
        dialog->Title = "Open student data file";
        dialog->Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";

        if (dialog->ShowDialog(this) != System::Windows::Forms::DialogResult::OK) {
            return;
        }

        try {
            inputTextBox_->Text = File::ReadAllText(dialog->FileName);
            resultTextBox_->Clear();
            statusLabel_->Text = "Opened " + Path::GetFileName(dialog->FileName);
        }
        catch (Exception^ ex) {
            MessageBox::Show(ex->Message, "Student Grade Calculator",
                             MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    void saveStudentData(Object^, EventArgs^) {
        SaveFileDialog^ dialog = gcnew SaveFileDialog();
        dialog->Title = "Save student data";
        dialog->Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
        dialog->FileName = "Students.txt";

        if (dialog->ShowDialog(this) != System::Windows::Forms::DialogResult::OK) {
            return;
        }

        try {
            std::vector<Person> students = readStudentsFromInput();
            std::ostringstream output;
            saveStudentsToStream(output, students);
            File::WriteAllText(dialog->FileName, toManagedString(output.str()));
            statusLabel_->Text = "Saved student data";
        }
        catch (const std::exception& ex) {
            showError(ex);
        }
        catch (Exception^ ex) {
            MessageBox::Show(ex->Message, "Student Grade Calculator",
                             MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    void saveResults(Object^, EventArgs^) {
        if (String::IsNullOrWhiteSpace(resultTextBox_->Text)) {
            calculateBoth(nullptr, nullptr);
        }

        if (String::IsNullOrWhiteSpace(resultTextBox_->Text)) {
            return;
        }

        SaveFileDialog^ dialog = gcnew SaveFileDialog();
        dialog->Title = "Save result file";
        dialog->Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
        dialog->FileName = "Results.txt";

        if (dialog->ShowDialog(this) != System::Windows::Forms::DialogResult::OK) {
            return;
        }

        try {
            File::WriteAllText(dialog->FileName, resultTextBox_->Text);
            statusLabel_->Text = "Saved result file";
        }
        catch (Exception^ ex) {
            MessageBox::Show(ex->Message, "Student Grade Calculator",
                             MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    void showAbout(Object^, EventArgs^) {
        MessageBox::Show(
            "Student Grade Calculator SD1 v1.0\r\n"
            "Windows Forms GUI enhancement\r\n\r\n"
            "Creator: Ayman Rafid\r\n"
            "Organization: VVK",
            "About",
            MessageBoxButtons::OK,
            MessageBoxIcon::Information);
    }

    void exitApplication(Object^, EventArgs^) {
        Close();
    }
};

[STAThreadAttribute]
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew GradeCalcForm());
    return 0;
}
