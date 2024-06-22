pub fn split_string(string_to_split: &str, sep: &str) -> Vec<String> {
    let mut buffer = String::new();
    let mut str_iter = string_to_split.chars();
    let mut sep_iter = sep.chars();
    let first_sep_char = sep_iter.next().unwrap();
    let mut tokens: Vec<String> = Vec::new();
    while let Some(c) = str_iter.next() {
        if c == first_sep_char {
            let mut matching_sep_buffer = String::new();
            matching_sep_buffer.push(c);
            let mut matching = true;
            while matching {
                match (str_iter.next(), sep_iter.next()) {
                    (Some(str_c), Some(sep_c)) => {
                        if sep_c != str_c {
                            matching = false;
                        }
                        matching_sep_buffer.push(str_c);
                    }
                    (None, Some(_)) => matching = false,
                    (Some(str_c), None) => {
                        tokens.push(buffer.clone());
                        buffer.clear();
                        buffer.push(str_c);
                        break;
                    }
                    (None, None) => {
                        tokens.push(buffer.clone());
                        buffer.clear();
                        break;
                    }
                }
            }
            if !matching {
                buffer.push_str(&matching_sep_buffer.clone());
            }
            sep_iter = sep.chars();
            sep_iter.next();
        } else {
            buffer.push(c);
        }
    }
    if !buffer.is_empty() {
        tokens.push(buffer.clone());
    }
    tokens
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn split_single_char() {
        let split_result = split_string("This is a test of splitting", " ");
        assert_eq!(
            vec!["This", "is", "a", "test", "of", "splitting"],
            split_result
        );
    }

    #[test]
    fn split_multi_char() {
        let split_result = split_string("This|||is|||an|||example|||of|||splitting|||", "|||");
        assert_eq!(
            vec!["This", "is", "an", "example", "of", "splitting"],
            split_result
        )
    }
}
