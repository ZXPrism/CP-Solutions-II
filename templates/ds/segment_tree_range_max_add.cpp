#include <vector>

// constraints
// 1. 1-indexed i.e. [1, len]
// 2. len should be positive
// 3. 1 <= left <= right <= len
// 4. vector init ignores vec[0]; len = vec.size() - 1, so size >= 2
// Invalid lengths, vectors, and ranges must throw an exception.

template<typename Ty>
class SegmentTreeRangeMaxAdd {
public:
	// zero init
	SegmentTreeRangeMaxAdd(int len);

	// init with a vector
	explicit SegmentTreeRangeMaxAdd(const std::vector<Ty> &vec);

	// [left, right]
	Ty range_max(int left, int right);

	// [left, right]
	void range_add(int left, int right, Ty val);

private:
};

#ifdef CP_TEMPLATE_TEST
#	include <doctest/doctest.h>

#	include <algorithm>
#	include <random>
#	include <string>
#	include <utility>
#	include <vector>

// Run with: xmake check -P templates (from the repository root).
// Contract: nonempty trees, 1-based inclusive ranges, left <= right.
// Vector initialization ignores vec[0] and uses vec.size() - 1 elements.
// All arithmetic in these tests stays within the value type's range.

namespace {
template<typename T>
void expect_max(SegmentTreeRangeMaxAdd<T> &tree, int left, int right,
                T expected, const std::string &context) {
	const T actual = tree.range_max(left, right);
	INFO(context);
	CAPTURE(left);
	CAPTURE(right);
	REQUIRE(actual == expected);
}

template<typename T>
void check_all(SegmentTreeRangeMaxAdd<T> &tree, const std::vector<T> &values,
               const std::string &context) {
	const int n = static_cast<int>(values.size()) - 1;
	for (int left = 1; left <= n; ++left) {
		T best = values[left];
		for (int right = left; right <= n; ++right) {
			best = std::max(best, values[right]);
			expect_max(tree, left, right, best, context);
		}
	}
}

template<typename T>
void add_and_check(SegmentTreeRangeMaxAdd<T> &tree, std::vector<T> &values,
                   int left, int right, T delta, const std::string &context) {
	tree.range_add(left, right, delta);
	for (int i = left; i <= right; ++i) values[i] += delta;
	check_all(tree, values, context);
}

TEST_CASE("SegmentTreeRangeMaxAdd: deterministic behavior") {
	SegmentTreeRangeMaxAdd<int> single(1);
	expect_max(single, 1, 1, 0, "singleton zero init");
	single.range_add(1, 1, -7);
	expect_max(single, 1, 1, -7, "singleton negative update");
	single.range_add(1, 1, 12);
	single.range_add(1, 1, 0);
	expect_max(single, 1, 1, 5, "singleton accumulated updates");

	const std::vector<int> one = { 999999, -13 };
	SegmentTreeRangeMaxAdd<int> from_one(one);
	expect_max(from_one, 1, 1, -13, "vector singleton ignores index zero");

	for (int n : { 2, 3, 5, 8, 9, 17 }) {
		SegmentTreeRangeMaxAdd<int> tree(n);
		std::vector<int> values(n + 1, 0);
		check_all(tree, values, "zero init n=" + std::to_string(n));
		add_and_check(tree, values, 1, n, -20, "full negative update");
		add_and_check(tree, values, 1, 1, 31, "first element update");
		add_and_check(tree, values, n, n, 40, "last element update");
		add_and_check(tree, values, 1, n, 0, "zero update");
	}

	const std::vector<int> input = { 999999, -8, -3, -11, -3, -20, -6, -9 };
	auto values = input;
	SegmentTreeRangeMaxAdd<int> tree(input);
	check_all(tree, values, "negative vector and tied maxima");
	add_and_check(tree, values, 1, 7, 10, "full update before partial access");
	add_and_check(tree, values, 2, 6, -17, "nested update");
	add_and_check(tree, values, 4, 7, 23, "overlapping update");
	add_and_check(tree, values, 3, 3, 100, "new maximum at interior point");
	add_and_check(tree, values, 3, 3, -200, "remove former maximum");
	add_and_check(tree, values, 1, 2, -9, "left disjoint update");
	add_and_check(tree, values, 6, 7, 14, "right disjoint update");

	// Several updates without queries in between exercise deferred updates.
	SegmentTreeRangeMaxAdd<int> deferred(7);
	deferred.range_add(1, 7, 12);
	deferred.range_add(1, 7, -5);
	deferred.range_add(2, 6, 8);
	deferred.range_add(3, 5, -20);
	deferred.range_add(1, 7, -4);
	check_all(deferred, std::vector<int>{ 0, 3, 11, -9, -9, -9, 11, 3 },
	          "batched nested updates");
	check_all(tree, values, "independent instances retain their state");

	std::vector<long long> wide = { 9000000000000LL, -5000000000LL,
		                            4000000000LL, -7000000000LL };
	SegmentTreeRangeMaxAdd<long long> wide_tree(wide);
	check_all(wide_tree, wide, "64-bit initialization");
	add_and_check(wide_tree, wide, 1, 3, 6000000000LL, "64-bit addition");
	add_and_check(wide_tree, wide, 2, 2, -20000000000LL, "64-bit negative addition");
	SegmentTreeRangeMaxAdd<long long> wide_zero(2);
	wide_zero.range_add(1, 2, -6000000000LL);
	expect_max(wide_zero, 1, 2, -6000000000LL, "64-bit zero constructor");
}

TEST_CASE("SegmentTreeRangeMaxAdd: randomized comparison with a vector") {
	constexpr unsigned seed = 0x5EED1234u;
	std::mt19937 rng(seed);
	auto pick = [&](int low, int high) {
		return std::uniform_int_distribution<int>(low, high)(rng);
	};
	for (int n : { 1, 2, 3, 7, 8, 9, 16, 17, 31, 32, 33 }) {
		for (int mode = 0; mode < 2; ++mode) {
			std::vector<int> values(n + 1, 0);
			values[0] = 1000000000;  // Must never affect a query.
			if (mode == 1) {
				for (int i = 1; i <= n; ++i) values[i] = pick(-1000, 1000);
			}
			auto tree = mode == 0 ? SegmentTreeRangeMaxAdd<int>(n)
			                      : SegmentTreeRangeMaxAdd<int>(values);
			const std::string label = "seed=" + std::to_string(seed) + " n=" + std::to_string(n) + " mode=" + std::to_string(mode);
			check_all(tree, values, label + " initial");
			for (int step = 0; step < 600; ++step) {
				int left = pick(1, n), right = pick(1, n);
				if (left > right) std::swap(left, right);
				const auto context = label + " step=" + std::to_string(step);
				if (pick(0, 3) != 0) {
					const int delta = pick(-1000, 1000);
					tree.range_add(left, right, delta);
					for (int i = left; i <= right; ++i) values[i] += delta;
				} else {
					const int expected = *std::max_element(values.begin() + left,
					                                       values.begin() + right + 1);
					expect_max(tree, left, right, expected, context);
				}
				if (step % 47 == 0) check_all(tree, values, context);
			}
			check_all(tree, values, label + " final");
		}
	}
}
}  // namespace

TEST_CASE("SegmentTreeRangeMaxAdd: invalid constructors throw") {
	CHECK_THROWS(SegmentTreeRangeMaxAdd<int>(0));
	CHECK_THROWS(SegmentTreeRangeMaxAdd<int>(-1));
	CHECK_THROWS(SegmentTreeRangeMaxAdd<int>(std::vector<int>{}));
	CHECK_THROWS(SegmentTreeRangeMaxAdd<int>(std::vector<int>{ 999 }));
}

TEST_CASE("SegmentTreeRangeMaxAdd: invalid ranges throw") {
	const std::vector<std::pair<int, int>> invalid = {
		{ 0, 1 }, { -1, 2 }, { 1, 6 }, { 6, 6 }, { 0, 0 }, { -2, -1 }, { 4, 3 }, { 1, 0 }, { 0, 6 }
	};
	for (auto [left, right] : invalid) {
		CAPTURE(left);
		CAPTURE(right);
		// Fresh instances: no extra promise about state after an exception.
		SegmentTreeRangeMaxAdd<int> query_tree(5);
		CHECK_THROWS(query_tree.range_max(left, right));
		SegmentTreeRangeMaxAdd<int> update_tree(5);
		CHECK_THROWS(update_tree.range_add(left, right, 7));
		SegmentTreeRangeMaxAdd<int> zero_update_tree(5);
		CHECK_THROWS(zero_update_tree.range_add(left, right, 0));
	}
	SegmentTreeRangeMaxAdd<int> from_vector(std::vector<int>{ 999, 4, 7 });
	CHECK_THROWS(from_vector.range_max(1, 3));
	CHECK_THROWS(from_vector.range_add(1, 3, 1));
}
#endif  // CP_TEMPLATE_TEST
