#ifndef SRC_ISORTER_HPP_
#define SRC_ISORTER_HPP_

#include "Sequence.hpp"

template <typename T>
void swap(T &first, T &other) {
    T tmp = std::move(first);
    first = std::move(other);
    other = std::move(tmp);
}

class Isorter {
public:
    template <class Contain, typename T>
    static Contain *bubble_sort(Contain *source, bool (*cmpf)(T, T)) {
        assert(source != NULL);

        for (std::size_t i = 0; i < source->get_size(); i++) {
            for (std::size_t j = 0; j < source->get_size() - i - 1; j++) {
                if (cmpf(source->operator[](j), source->operator[](j + 1)))
                    swap(source->operator[](j), source->operator[](j + 1));
            }
        }

        return source;
    }

    template <class Contain, typename T>
    static Contain *shaker_sort(Contain *source, bool (*cmpf)(T, T)) {
        assert(source != NULL);

        std::size_t control = source->get_size() - 1, left = 0,
                    right = source->get_size() - 1;

        do {
            for (std::size_t i = left; i < right; i++) {
                if (cmpf(source->operator[](i), source->operator[](i + 1))) {
                    swap(source->operator[](i), source->operator[](i + 1));
                    control = i;
                }
            }

            right = control;

            for (std::size_t i = right; i > left; i--) {
                if (cmpf(source->operator[](i - 1), source->operator[](i))) {
                    swap(source->operator[](i - 1), source->operator[](i));
                    control = i;
                }
            }

            left = control;
        } while (left < right);

        return source;
    }

    template <class Contain, typename T>
    static Contain *qsort(Contain *source, bool (*cmpf)(T, T),
                          const std::size_t left, const std::size_t right) {
        assert(source != NULL);

        if (left < right) {
            std::size_t pivot = partition(source, cmpf, left, right);

            qsort(source, cmpf, left, pivot - 1);
            qsort(source, cmpf, pivot + 1, right);
        }

        return source;
    }

    template <class Contain, typename T>
    static std::size_t partition(Contain *source, bool (*cmpf)(T, T),
                                 std::size_t left, std::size_t right) {
        std::size_t i = left - 1, j = left;
        T pivot = source->operator[](right);

        for (; j < right; j++) {
            if (cmpf(source->operator[](j), pivot)) {
                i++;
                swap(source->operator[](i), source->operator[](j));
            }
        }

        swap(source->operator[](j), source->operator[](right));

        return j - 1;
    }

    template <class Contain, typename T>
    static void merge_sort(Contain *source, bool (*cmpf)(T, T),
                           const std::size_t &begin, const std::size_t &end) {
        if (begin >= end) {
            return;
        }

        std::size_t mid = begin + (end - begin) / 2;
        merge_sort(source, cmpf, begin, mid);
        merge_sort(source, cmpf, mid + 1, end);
        merge(source, cmpf, begin, mid, end);
    }

    template <class Contain, typename T>
    static void merge(Contain *source, bool (*cmpf)(T, T),
                      const std::size_t &left, const std::size_t &mid,
                      const std::size_t &right) {
        std::size_t first_subarray_length = mid - left + 1;
        std::size_t second_subarray_length = right - mid;

        auto left_subarray = new Contain(first_subarray_length);
        auto right_subarray = new Contain(second_subarray_length);

        for (std::size_t i = 0; i < first_subarray_length; i++) {
            left_subarray->append(source->operator[](left + i));
        }
        for (std::size_t i = 0; i < second_subarray_length; i++) {
            right_subarray->append(source->operator[](mid + i + 1));
        }

        std::size_t index_first_subarray = 0, index_second_subarray = 0,
                    index_merged = left;

        while ((index_first_subarray < first_subarray_length) &&
               (index_second_subarray < second_subarray_length)) {
            if (!cmpf(left_subarray->operator[](index_first_subarray),
                      right_subarray->operator[](index_second_subarray))) {
                source->operator[](index_merged++) =
                    left_subarray->operator[](index_first_subarray++);
            } else {
                source->operator[](index_merged++) =
                    right_subarray->operator[](index_second_subarray++);
            }
        }

        while (index_first_subarray < first_subarray_length) {
            source->operator[](index_merged++) =
                left_subarray->operator[](index_first_subarray++);
        }

        while (index_second_subarray < second_subarray_length) {
            source->operator[](index_merged++) =
                right_subarray->operator[](index_second_subarray++);
        }

        delete left_subarray;
        delete right_subarray;
    }
};

class Isorter_list {
public:
    template <typename T>
    using literator = typename list_sequence<T>::list_iterator;

    template <typename T>
    static void merge_sort_list(list_sequence<T> *source, bool (*cmpf)(T, T)) {
        literator<T> source_to_merge = source->begin();
        merge_sort_list_recursive(source_to_merge, cmpf);
    }

    template <typename T>
    static void merge_sort_list_recursive(literator<T> &source,
                                          bool (*cmpf)(T, T)) {
        literator<T> head = source;
        literator<T> next = source;
        next.operator++();
        literator<T> first_sublist(NULL), second_sublist(NULL);

        if (head.current_ptr() == NULL || next.current_ptr() == NULL) {
            return;
        }

        Isorter_list::spliting_list<T>(head, first_sublist, second_sublist);

        merge_sort_list_recursive(first_sublist, cmpf);
        merge_sort_list_recursive(second_sublist, cmpf);

        head.set_ptr(Isorter_list::merge_sorted_lists(first_sublist,
                                                      second_sublist, cmpf));
        source = head;
    }

    template <typename T>
    static void spliting_list(literator<T> head, literator<T> &first_sublist,
                              literator<T> &second_sublist) {
        literator<T> slow = head, fast = head;
        fast.operator++();

        while (fast.current_ptr() != NULL) {
            fast.operator++();

            if (fast.current_ptr() != NULL) {
                fast.operator++();
                slow.operator++();
            }
        }

        literator<T> copy = slow;
        copy.operator++();
        slow.current_ptr()->next = NULL;
        first_sublist = head;
        second_sublist = copy;
    }

    template <typename T>
    static node<T> *merge_sorted_lists(literator<T> first_sublist,
                                       literator<T> second_sublist,
                                       bool (*cmpf)(T, T)) {
        literator<T> result = first_sublist;
        // std::cout << result.get_value();

        if (first_sublist.current_ptr() == NULL) {
            return second_sublist.current_ptr();
        } else if (second_sublist.current_ptr() == NULL) {
            return first_sublist.current_ptr();
        }

        if (cmpf(first_sublist.get_value(), second_sublist.get_value())) {
            result.set_ptr(first_sublist.current_ptr());
            result.current_ptr()->next = Isorter_list::merge_sorted_lists<T>(
                first_sublist.operator++(), second_sublist, cmpf);
        } else {
            result.set_ptr(second_sublist.current_ptr());
            result.current_ptr()->next = Isorter_list::merge_sorted_lists<T>(
                first_sublist, second_sublist.operator++(), cmpf);
        }

        return result.current_ptr();
    }
};

bool compare_int(int first, int other) { return first > other; }

#endif  // SRC_ISORTER_HPP_
