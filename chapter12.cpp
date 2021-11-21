//
// Created by jewoo on 2021-11-21.
//
// 해시 테이블
#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_map>
#include <unordered_set>


namespace ch12 {

    int StringHash(const std::string &s, int modulus) {
        const int kMult = 997;
        return std::accumulate(std::begin(s), std::end(s), 0,
                               [kMult, modulus](int val, char c) {
                                   return (val * kMult + c) % modulus;
                               });
    }

    std::vector<std::vector<std::string >> FindAnagrams(const std::vector<std::string> &dictionary) {
        std::unordered_map<std::string, std::vector<std::string >> sored_string_to_anagrams;
        for (const std::string &s: dictionary) {
            //
            //
            std::string sorted_str(s);
            std::sort(std::begin(sorted_str), std::end(sorted_str));
            sored_string_to_anagrams[sorted_str].emplace_back(s);
        }
        std::vector<std::vector<std::string >> anagram_groups;
        for (const auto &[key, group]: sored_string_to_anagrams) {
            if (std::size(group) >= 2) {
                anagram_groups.emplace_back(group);
            }
        }
        return anagram_groups;
    }

    // 해시 테이블 클래스 설계하기
    struct ContactList {
        // 동등성 함수
        bool operator==(const ContactList &that) const {
            return std::unordered_set<std::string>(std::begin(names), std::end(names))
                   == std::unordered_set<std::string>(std::begin(that.names), std::end(that.names));
        }

        std::vector<std::string> names;
    };

    // ContactList 에 대한 해시 함수
    struct HashContactList {
        size_t operator()(const ContactList &contacts) const {
            size_t hash_code{0};
            for (const std::string &name:
                    std::unordered_set<std::string>(std::begin(contacts.names),
                                                    std::end(contacts.names))) {
                hash_code ^= std::hash<std::string>()(name);
            }
            return hash_code;
        }
    };

    std::vector<ContactList> MergeContactLists(const std::vector<ContactList> &contacts) {
        std::unordered_set<ContactList, HashContactList> unique_contacts(std::begin(contacts),
                                                                         std::end(contacts));
        return {std::begin(unique_contacts), std::end(unique_contacts)};
    }

    // 12.1 회문 순열 확인하기
    // 회문: 앞으로 읽을 때와 뒤로 읽을 때가 같은 문자열을 말한다.
    bool CanFormPalindrome(const std::string &s) {

        std::unordered_set<char> chars_with_odd_frequency;
        for (char c: s) {
            if (chars_with_odd_frequency.count(c)) {
                //
                chars_with_odd_frequency.erase(c);
            } else {
                chars_with_odd_frequency.emplace(c);
            }
        }
        return std::size(chars_with_odd_frequency) <= 1;
    }

    // 12.2 익명의 편지 작성하기
    bool IsLetterConstructibleMagazine(const std::string &letter_text,
                                       const std::string &magazine_text) {
        std::unordered_map<char, int> char_frequency_for_letter;

        for (char c: letter_text) {
            ++char_frequency_for_letter[c];
        }

        for (char c: magazine_text) {
            if (auto it = char_frequency_for_letter.find(c); it != std::cend(char_frequency_for_letter)) {
                --it->second;
                if (it->second == 0) {
                    char_frequency_for_letter.erase(it);
                    if (std::empty(char_frequency_for_letter)) {
                        break;
                    }
                }
            }
        }
        return std::empty(char_frequency_for_letter);
    }
}