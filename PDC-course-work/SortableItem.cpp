#include "SortableItem.h"

SortableItem::SortableItem(int firstIntValue, int secondIntValue, double doubleValue) :
    firstIntValue(firstIntValue), secondIntValue(secondIntValue), doubleValue(doubleValue) {}

bool SortableItem::operator<(const SortableItem& otherItem) const {
    if (firstIntValue != otherItem.firstIntValue)
        return firstIntValue < otherItem.firstIntValue;
    if (secondIntValue != otherItem.secondIntValue)
        return secondIntValue < otherItem.secondIntValue;
    return doubleValue < otherItem.doubleValue;
}

bool SortableItem::operator>(const SortableItem& otherItem) const {
    return otherItem < *this;
}

bool SortableItem::operator<=(const SortableItem& otherItem) const {
    return !(*this > otherItem);
}

bool SortableItem::operator>=(const SortableItem& otherItem) const {
    return !(*this < otherItem);
}

bool SortableItem::operator==(const SortableItem& otherItem) const {
    return (firstIntValue == otherItem.firstIntValue
        && secondIntValue == otherItem.secondIntValue
        && doubleValue == otherItem.doubleValue);
}

void SortableItem::print() const {
    std::cout << "first int: " << firstIntValue << ", secont int: " << secondIntValue << ", double: " << doubleValue << std::endl;
}

SortableItem SortableItem::generateRandom() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> firstIntDist(0, 10000);
    std::uniform_int_distribution<int> secondIntDist(0, 100000);
    std::uniform_real_distribution<double> doubleDist(0.0, 1000.0);

    int randomFirstInt = firstIntDist(gen);
    int randomSecondInt = secondIntDist(gen);
    double randomDouble = doubleDist(gen);

    return SortableItem(randomFirstInt, randomSecondInt, randomDouble);
}