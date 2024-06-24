use std::fs;
use std::usize;

fn main() {
    let input = fs::read_to_string("inputs/day_5/part_1.txt").unwrap();
    let mut nice_string_count: u32 = 0;
    for line in input.lines() {
        if is_string_nice(line) {
            nice_string_count += 1;
        }
    }
    println!("The number of nice strings is {nice_string_count}");
}

fn is_string_nice(s: &str) -> bool {
    const NICE_STRING_VOWEL_COUNT: u32 = 3;
    const NICE_STRING_CHAR_REPETITION: usize = 2;
    const NICE_STRING_FORBIDDEN_SEQUENCES: [&str; 4] = ["ab", "cd", "pq", "xy"];
    let is_vowel_count_correct = count_vowels(s) >= NICE_STRING_VOWEL_COUNT;
    let is_repeated_char = is_char_repeated(s, NICE_STRING_CHAR_REPETITION);
    let has_forbidden_sequences =
        is_forbidden_sequence_contained(s, &NICE_STRING_FORBIDDEN_SEQUENCES);

    is_vowel_count_correct && is_repeated_char && !has_forbidden_sequences
}

fn count_vowels(s: &str) -> u32 {
    let mut vowel_count = 0;
    for c in s.chars() {
        match c {
            'a' | 'e' | 'i' | 'o' | 'u' => vowel_count += 1,
            _ => (),
        }
    }
    vowel_count
}

fn is_char_repeated(s: &str, target_repetitions: usize) -> bool {
    let mut repeated_times = 0;
    let mut previous_char = ' ';
    for c in s.chars() {
        if c == previous_char {
            repeated_times += 1;
        } else {
            repeated_times = 1;
            previous_char = c;
        }
        if repeated_times == target_repetitions {
            break;
        }
    }
    repeated_times == target_repetitions
}

fn is_forbidden_sequence_contained(s: &str, forbidden_sequences: &[&str]) -> bool {
    for sequence in forbidden_sequences {
        if s.contains(sequence) {
            return true;
        }
    }
    false
}

#[cfg(test)]
mod tests {

    use super::*;
    #[test]
    fn vowel_count() {
        let string_1 = "aei";
        let string_2 = "ugknbfddgicrmopn";
        let string_3 = "dcszwmarrgswjxmb";

        assert_eq!(count_vowels(string_1), 3);
        assert_eq!(count_vowels(string_2), 3);
        assert_eq!(count_vowels(string_3), 1);
    }

    #[test]
    fn charater_repetition() {
        let string_1 = "abcdde";
        let string_2 = "abbccddeeaaa";
        let string_3 = "aabbccddeeedddfff";

        assert_eq!(is_char_repeated(string_1, 2), true);
        assert_eq!(is_char_repeated(string_2, 3), true);
        assert_eq!(is_char_repeated(string_3, 4), false);
    }

    #[test]
    fn forbidden_sequences() {
        let string_1 = "ugknbfddgicrmopn";
        let string_2 = "haegwjzuvuyypxyu";
        let string_3 = "jchzalrnumimnmhp";

        let forbidden_sequences = vec!["ab", "cd", "pq", "xy"];
        assert_eq!(
            is_forbidden_sequence_contained(string_1, &forbidden_sequences),
            false
        );
        assert_eq!(
            is_forbidden_sequence_contained(string_2, &forbidden_sequences),
            true
        );
        assert_eq!(
            is_forbidden_sequence_contained(string_3, &forbidden_sequences),
            false
        );
    }
}
