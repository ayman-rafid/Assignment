#pragma once

enum class SplitStrategy {
    Strategy1 = 1,
    Strategy2 = 2
};

void benchmarkAllContainers(SplitStrategy strategy);