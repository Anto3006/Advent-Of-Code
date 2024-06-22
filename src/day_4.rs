use md5;
use std::process;

fn main() {
    let number_zeros = 6;
    let value = calculate_min_MD5_zeros("bgvyzdsv", number_zeros);
    println!("The minimum number to obtain a hash with {number_zeros} zeros at the beginning of the hexadecimal representation is {value}");
}

fn calculate_min_MD5_zeros(key: &str, number_zeros: u32) -> u32 {
    let mut number = 0;
    let mut found_zeros_required = false;
    while !found_zeros_required {
        let full_key = format!("{key}{number}");
        let md5_hash = md5::compute(full_key);
        let mut zero_count = 0;
        for byte in md5_hash.iter() {
            if *byte == 0 {
                zero_count += 2;
            } else {
                let upper_hex = byte / 16;
                if upper_hex == 0 {
                    zero_count += 1;
                }
                break;
            }
        }
        if zero_count >= number_zeros {
            found_zeros_required = true;
        } else {
            number += 1;
        }
    }
    number
}

#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn check_five_md5_five_zeros() {
        let key_1 = "abcdef";
        let key_2 = "pqrstuv";

        assert_eq!(calculate_min_MD5_zeros(key_1, 5), 609043);
        assert_eq!(calculate_min_MD5_zeros(key_2, 5), 1048970);
    }
}
