fn main() {
    let first = letters_to_numeric("hepxxzaa");
    let forbidden = vec!['i' as u8, 'o' as u8, 'l' as u8];
    let password_gen = PasswordGenerator::new(8, 'z' as u8, 'a' as u8, Some(first));
    for password in password_gen {
        if check_forbidden(&password, &forbidden)
            && check_increasing_seq(&password, 3)
            && number_pairs(&password) >= 2
        {
            println!("{}", numeric_to_letters(&password));
            break;
        }
    }
    println!("This is day 11 of advent of code 2015");
}

fn check_increasing_seq(password: &[u8], length: usize) -> bool {
    let mut max_length = 0;
    let mut current_length = 0;
    let mut prev_letter = 0;
    for letter in password {
        if *letter != prev_letter + 1 {
            if current_length > max_length {
                max_length = current_length;
            }
            current_length = 1;
        } else {
            current_length += 1;
        }
        prev_letter = *letter;
    }
    if current_length > max_length {
        max_length = current_length;
    }
    max_length >= length
}

fn check_forbidden(password: &[u8], forbidden: &[u8]) -> bool {
    for value in password {
        if forbidden.contains(&value) {
            return false;
        }
    }
    true
}

fn number_pairs(password: &[u8]) -> usize {
    let mut pair_count = 0;
    let mut previous_value = 0;
    let mut can_be_pair = false;
    for value in password {
        if can_be_pair {
            if *value == previous_value {
                pair_count += 1;
                can_be_pair = false;
            }
        } else {
            can_be_pair = true;
        }
        previous_value = *value
    }
    pair_count
}

fn letters_to_numeric(text: &str) -> Vec<u8> {
    let mut result = Vec::new();
    for c in text.chars() {
        result.push(c as u8)
    }
    result
}

fn numeric_to_letters(numbers: &[u8]) -> String {
    let mut result = String::new();
    for c in numbers {
        result.push(*c as char);
    }
    result
}

struct PasswordGenerator {
    size: usize,
    current_password: Vec<u8>,
    max_value: u8,
    min_value: u8,
    first_password: bool,
}

impl PasswordGenerator {
    fn new(size: usize, max_value: u8, min_value: u8, start_point: Option<Vec<u8>>) -> Self {
        if let Some(first) = start_point {
            Self {
                size,
                current_password: first,
                first_password: true,
                max_value,
                min_value,
            }
        } else {
            let first = vec![min_value; size];
            Self {
                size,
                current_password: first,
                first_password: true,
                max_value,
                min_value,
            }
        }
    }
}

impl Iterator for PasswordGenerator {
    type Item = Vec<u8>;
    fn next(&mut self) -> Option<Self::Item> {
        if self.first_password {
            self.first_password = false;
            Some(self.current_password.clone())
        } else {
            let mut carry = 1;
            for pos in (0..self.size).rev() {
                let mut new_digit = self.current_password[pos] + carry;
                if new_digit > self.max_value {
                    new_digit = self.min_value;
                    carry = 1;
                } else {
                    carry = 0;
                }
                self.current_password[pos] = new_digit;
            }
            if carry == 1 {
                None
            } else {
                Some(self.current_password.clone())
            }
        }
    }
}
