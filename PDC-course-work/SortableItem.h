#pragma once

#include <iostream>
#include <random>

class SortableItem {
public:
	int firstIntValue;
	int secondIntValue;
	double doubleValue;

	SortableItem(int firstIntValue, int secondIntValue, double doubleValue);

	bool operator<(const SortableItem& otherItem) const;
	bool operator>(const SortableItem& otherItem) const;
	bool operator<=(const SortableItem& otherItem) const;
	bool operator>=(const SortableItem& otherItem) const;
	bool operator==(const SortableItem& otherItem) const;

	void print() const;

	static SortableItem generateRandom();
};