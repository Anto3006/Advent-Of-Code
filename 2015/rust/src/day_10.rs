fn main() {
    println!("This is advent of code 2015 day 10");
    let mut current_input = String::from("3113322113");
    let iter = 50;
    for _ in 0..iter {
        current_input = look_and_say(&current_input);
    }
    println!("{}", current_input.len());
}

fn look_and_say(text: &str) -> String {
    let mut result = String::default();
    let mut current_count = 0;
    let mut current_char = ' ';
    for c in text.chars() {
        if c != current_char {
            if current_count > 0 {
                result.push_str(&format!("{}{}", current_count, current_char));
            }
            current_char = c;
            current_count = 1;
        } else {
            current_count += 1;
        }
    }
    if current_count > 0 {
        result.push_str(&format!("{}{}", current_count, current_char));
    }
    result
}

#[cfg(test)]
#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn test_look_and_say() {
        assert_eq!(String::from("11"), look_and_say("1"));
        assert_eq!(String::from("21"), look_and_say("11"));
        assert_eq!(String::from("1211"), look_and_say("21"));
        assert_eq!(String::from("111221"), look_and_say("1211"));
        assert_eq!(String::from("312211"), look_and_say("111221"));
    }
}
