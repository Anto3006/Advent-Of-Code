use aoc;
use std::collections::HashMap;
use std::fs;

fn main() {
    let input = fs::read_to_string("inputs/day_13/input.txt").unwrap();
    let mut table_situation = parse_happiness(&input);
    table_situation.add_neutral();
    let permutations = aoc::generate_permutations(&table_situation.names);
    let mut max_evaluation = 0;
    for seating in permutations {
        let evaluation = table_situation.evaluate_seating(seating);
        if evaluation > max_evaluation {
            max_evaluation = evaluation;
        }
    }
    println!("{}", max_evaluation);
}

struct TableSituation {
    names: Vec<String>,
    happiness: HashMap<(String, String), i32>,
}

impl TableSituation {
    fn evaluate_seating(&self, seating: Vec<String>) -> i32 {
        let mut total_happiness = 0;
        for (pos, name) in seating.iter().enumerate() {
            let prev_position = if pos == 0 { seating.len() - 1 } else { pos - 1 };
            let next_position = if pos == seating.len() - 1 { 0 } else { pos + 1 };

            let prev_happiness = *self
                .happiness
                .get(&(name.clone(), seating[prev_position].clone()))
                .unwrap();
            let next_happiness = *self
                .happiness
                .get(&(name.clone(), seating[next_position].clone()))
                .unwrap();
            total_happiness += prev_happiness + next_happiness;
        }
        total_happiness
    }

    fn add_neutral(&mut self) {
        let neutral_name = "Neutral".to_string();
        for name in self.names.iter() {
            self.happiness
                .insert((neutral_name.clone(), name.clone()), 0);
            self.happiness
                .insert((name.clone(), neutral_name.clone()), 0);
        }
        self.names.push(neutral_name.clone());
    }
}

fn parse_happiness(text: &str) -> TableSituation {
    let mut happiness = HashMap::new();
    let mut names = Vec::new();
    for line in text.lines() {
        let tokens = aoc::split_string(line, " ");
        let initial_name = tokens[0].clone();
        if !names.contains(&initial_name) {
            names.push(initial_name.clone());
        }
        let sign: i32 = if tokens[2] == "gain" { 1 } else { -1 };
        let happiness_value = sign * tokens[3].parse::<i32>().unwrap();
        let len = tokens[10].len();
        let target_name = tokens[10][0..len - 1].to_string();
        happiness.insert((initial_name, target_name), happiness_value);
    }
    TableSituation { names, happiness }
}
