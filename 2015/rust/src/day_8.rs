use std::fs;

fn main() {
    println!("This is day 8 of advent of code 2015");
    let input = fs::read_to_string("inputs/day_8/input.txt").unwrap();
    let mut diff = 0;
    for line in input.lines() {
        diff += calculate_encoded_string_size(line) - line.chars().count();
    }
    println!("Result: {diff}");
}

fn count_chars(str: &str) -> usize {
    let mut count = 0;
    let mut is_char_escaped = false;
    let mut is_hex = false;
    for c in str.chars() {
        if !is_char_escaped {
            if c == '\\' {
                is_char_escaped = true;
            } else {
                count += 1;
            }
        } else {
            if !is_hex {
                if c == 'x' {
                    is_hex = true;
                } else {
                    is_char_escaped = false;
                    count += 1;
                }
            } else {
                is_hex = false
            }
        }
    }
    count -= 2;
    count
}

fn calculate_encoded_string_size(string: &str) -> usize {
    let mut size = 0;
    for c in string.chars() {
        if c == '\"' || c == '\\' {
            size += 2;
        } else {
            size += 1;
        }
    }
    size += 2;
    size
}

#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn test_count_chars() {
        let first_test = "\"\"";
        let second_test = "\"aaa\"";
        let third_test = "\"aaa\"aaa\"";
        let fourth_test = "\"a\\\\a\"";
        let fifth_test = "\"\\x23a\"";

        assert_eq!(count_chars(first_test), 0);
        assert_eq!(count_chars(second_test), 3);
        assert_eq!(count_chars(third_test), 7);
        assert_eq!(count_chars(fourth_test), 3);
        assert_eq!(count_chars(fifth_test), 2);
    }

    #[test]
    fn test_calculate_encoded_size() {
        let first_test = "\"\"";
        let second_test = "\"aaa\"";
        let third_test = "\"aaa\\\"aaa\"";
        let fourth_test = "\"a\\a\"";
        let fifth_test = "\"\\x23\"";

        assert_eq!(calculate_encoded_string_size(first_test), 6);
        assert_eq!(calculate_encoded_string_size(second_test), 9);
        assert_eq!(calculate_encoded_string_size(third_test), 16);
        assert_eq!(calculate_encoded_string_size(fourth_test), 10);
        assert_eq!(calculate_encoded_string_size(fifth_test), 11);
    }
}
