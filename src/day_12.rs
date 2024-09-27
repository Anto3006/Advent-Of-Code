use std::collections::HashMap;
use std::fs;

fn main() {
    let input = fs::read_to_string("inputs/day_12/input.txt").unwrap();
    println!("This is day 12 of advent of code 2015");
    let tokens = tokenize(&input);
    if let Some((json_array, _)) = parse_array(&tokens, 0) {
        let mut sum = 0;
        for json_element in json_array.array {
            sum += json_element.sum_values();
        }
        println!("Result: {}", sum);
    }
}

#[derive(Debug)]
enum JsonValue {
    Int(i32),
    Str(String),
}

#[derive(Debug)]
struct JsonObject {
    object: HashMap<String, JsonElement>,
}

impl JsonObject {
    fn is_ignored(&self) -> bool {
        self.object.values().any(|value| match value {
            JsonElement::Value(v) => match v {
                JsonValue::Str(text) => text == "red",
                _ => false,
            },
            _ => false,
        })
    }
}

#[derive(Debug)]
struct JsonArray {
    array: Vec<JsonElement>,
}

#[derive(Debug)]
enum JsonElement {
    Object(JsonObject),
    Array(JsonArray),
    Value(JsonValue),
}

impl JsonElement {
    fn sum_values(&self) -> i32 {
        match self {
            Self::Value(value) => match value {
                JsonValue::Str(_) => 0,
                JsonValue::Int(number) => *number,
            },
            Self::Array(json_array) => json_array
                .array
                .iter()
                .map(|element| element.sum_values())
                .sum(),
            Self::Object(json_object) => {
                if json_object.is_ignored() {
                    return 0;
                } else {
                    json_object
                        .object
                        .iter()
                        .map(|(_, value)| value.sum_values())
                        .sum()
                }
            }
        }
    }
}

#[derive(Debug)]
enum Token {
    OpenSquareBracket,
    CloseSquareBracket,
    OpenCurlyBracket,
    CloseCurlyBracket,
    Colon,
    Comma,
    PlainText(String),
    Number(i32),
}

fn tokenize(text: &str) -> Vec<Token> {
    let mut text_value = String::new();
    let mut number_value: Option<i32> = None;
    let mut is_parsing_text = false;
    let mut is_negative = false;
    let mut tokens = Vec::new();
    for c in text.chars() {
        if is_parsing_text {
            if c == '"' {
                tokens.push(Token::PlainText(text_value.clone()));
                text_value.clear();
                is_parsing_text = false;
            } else {
                text_value.push(c);
            }
        } else if c.is_digit(10) {
            if let Some(number) = number_value {
                number_value = Some(number * 10 + c.to_digit(10).unwrap() as i32);
            } else {
                number_value = Some(c.to_digit(10).unwrap() as i32)
            }
        } else {
            if let Some(mut number) = number_value {
                if is_negative {
                    number *= -1;
                }
                tokens.push(Token::Number(number));
                number_value = None;
            }
            is_negative = false;
            if c == '[' {
                tokens.push(Token::OpenSquareBracket);
            } else if c == ']' {
                tokens.push(Token::CloseSquareBracket);
            } else if c == '{' {
                tokens.push(Token::OpenCurlyBracket);
            } else if c == '}' {
                tokens.push(Token::CloseCurlyBracket);
            } else if c == ',' {
                tokens.push(Token::Comma);
            } else if c == ':' {
                tokens.push(Token::Colon);
            } else if c == '"' {
                is_parsing_text = true;
            } else if c == '-' {
                is_negative = true;
            }
        }
    }
    tokens
}

fn parse_array(tokens: &Vec<Token>, initial_position: usize) -> Option<(JsonArray, usize)> {
    let mut array = vec![];
    if let Token::OpenSquareBracket = tokens[initial_position] {
        let mut position = initial_position + 1;
        let mut found_end_array = false;
        while !found_end_array {
            match &tokens[position] {
                Token::CloseSquareBracket => {
                    found_end_array = true;
                    position += 1;
                }
                Token::Number(number) => {
                    array.push(JsonElement::Value(JsonValue::Int(*number)));
                    position += 1;
                }
                Token::PlainText(text) => {
                    array.push(JsonElement::Value(JsonValue::Str(text.clone())));
                    position += 1;
                }
                Token::OpenSquareBracket => {
                    if let Some((json_array, new_position)) = parse_array(tokens, position) {
                        array.push(JsonElement::Array(json_array));
                        position = new_position;
                    } else {
                        eprintln!("Error when parsing");
                        return None;
                    }
                }
                Token::OpenCurlyBracket => {
                    if let Some((json_object, new_position)) = parse_object(tokens, position) {
                        array.push(JsonElement::Object(json_object));
                        position = new_position;
                    } else {
                        eprintln!("Error when parsing");
                        return None;
                    }
                }
                Token::CloseCurlyBracket => {
                    return None;
                }
                Token::Comma => {
                    position += 1;
                }
                Token::Colon => {
                    return None;
                }
            }
        }
        return Some((JsonArray { array }, position));
    } else {
        eprintln!("Error when parsing: First element is not valid");
        return None;
    }
}

fn parse_object(tokens: &Vec<Token>, initial_position: usize) -> Option<(JsonObject, usize)> {
    let mut object = HashMap::new();
    if let Token::OpenCurlyBracket = tokens[initial_position] {
        let mut position = initial_position + 1;
        let mut found_end_object = false;
        let mut key = String::from("");
        let mut is_reading_value = false;
        let mut key_read = false;
        while !found_end_object {
            match &tokens[position] {
                Token::CloseCurlyBracket => {
                    found_end_object = true;
                    position += 1;
                }
                Token::Number(number) => {
                    if is_reading_value {
                        object.insert(key, JsonElement::Value(JsonValue::Int(*number)));
                        key = "".to_string();
                        key_read = false;
                        is_reading_value = false;
                    } else {
                        return None;
                    }
                    position += 1;
                }
                Token::PlainText(text) => {
                    if is_reading_value {
                        object.insert(key, JsonElement::Value(JsonValue::Str(text.clone())));
                        key = "".to_string();
                        key_read = false;
                        is_reading_value = false;
                    } else {
                        key = text.clone();
                        key_read = true;
                    }
                    position += 1;
                }
                Token::OpenSquareBracket => {
                    if is_reading_value {
                        if let Some((json_array, new_position)) = parse_array(tokens, position) {
                            object.insert(key, JsonElement::Array(json_array));
                            position = new_position;
                            key_read = false;
                            key = "".to_string();
                            is_reading_value = false;
                        } else {
                            eprintln!("Error when parsing");
                            return None;
                        }
                    } else {
                        return None;
                    }
                }
                Token::OpenCurlyBracket => {
                    if is_reading_value {
                        if let Some((json_object, new_position)) = parse_object(tokens, position) {
                            object.insert(key, JsonElement::Object(json_object));
                            position = new_position;
                            key_read = false;
                            key = "".to_string();
                            is_reading_value = false;
                        } else {
                            eprintln!("Error when parsing");
                            return None;
                        }
                    } else {
                        eprintln!("Error when parsing");
                        return None;
                    }
                }
                Token::CloseSquareBracket => {
                    eprintln!("Error when parsing");
                    return None;
                }
                Token::Comma => {
                    position += 1;
                }
                Token::Colon => {
                    if key_read {
                        is_reading_value = true;
                    } else {
                        eprintln!("Error when parsing: misplaced colon");
                        return None;
                    }
                    position += 1;
                }
            }
        }
        return Some((JsonObject { object }, position));
    } else {
        eprintln!("Error when parsing: First element is not valid");
        return None;
    }
}

fn sum_numbers_text(text: &str) -> i32 {
    let mut sum: i32 = 0;
    let mut is_parsing_number = false;
    let mut current_number = 0;
    let mut is_negative = false;
    for c in text.chars() {
        if c == '-' {
            is_negative = true;
        } else if c.is_numeric() {
            if is_parsing_number {
                current_number = current_number * 10 + c.to_digit(10).unwrap();
            } else {
                current_number = c.to_digit(10).unwrap();
                is_parsing_number = true;
            }
        } else {
            if is_parsing_number {
                if is_negative {
                    sum -= i32::try_from(current_number).unwrap();
                } else {
                    sum += i32::try_from(current_number).unwrap();
                }
                current_number = 0;
                is_parsing_number = false;
            }
            is_negative = false;
        }
    }
    sum
}
