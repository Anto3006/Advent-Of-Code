use std::fs;

fn main() {
    println!("This is day 8 of advent of code 2015");
    let input = fs::read_to_string("inputs/day_8/part_1.txt").unwrap();
    let mut diff = 0;
    for line in input.lines() {
        diff += line.chars().count() - count_chars(line);
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
}
