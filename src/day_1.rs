use std::fs;

fn main() {
    let instructions = fs::read_to_string("inputs/day_1/part_1.txt").expect("cannot open file");

    let final_floor = calculate_floor(&instructions);
    println!("The final floor is {final_floor}");
}

fn calculate_floor(instructions: &str) -> i32 {
    let mut floor = 0;
    let mut first_basement_visit = false;
    for (pos, c) in instructions.chars().enumerate() {
        match c {
            '(' => floor += 1,
            ')' => floor -= 1,
            _ => eprintln!("Wrong character in input '{c}', it must be '(' or ')'"),
        }
        if !first_basement_visit && floor == -1 {
            println!("Got to basement for the first time in movement {}", pos + 1);
            first_basement_visit = true;
        }
    }
    floor
}

#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn is_calculated_floor_correct() {
        let instructions = fs::read_to_string("inputs/day_1/test_part_1.txt").unwrap();
        let floor_calculated = calculate_floor(&instructions);
        assert_eq!(floor_calculated, 1);
    }
}
