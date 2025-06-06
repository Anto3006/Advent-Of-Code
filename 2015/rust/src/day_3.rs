use std::collections::HashSet;
use std::fs;

fn main() {
    let movements = fs::read_to_string("inputs/day_3/part_1.txt").unwrap();
    let visited_houses_solo = calculate_visited_houses(&movements, 1);
    let visited_houses_pair = calculate_visited_houses(&movements, 2);
    println!("Santa visited {} houses", visited_houses_solo.len());
    println!(
        "Santa with Robo-Santa visited {} houses together",
        visited_houses_pair.len()
    );
}

fn calculate_visited_houses(movements: &str, number_participants: usize) -> HashSet<(i32, i32)> {
    let mut visited_houses = HashSet::new();
    let mut positions: Vec<(i32, i32)> = Vec::new();
    for _ in 0..number_participants {
        positions.push((0, 0));
    }
    let mut participant = 0;
    visited_houses.insert((0, 0));
    for c in movements.chars() {
        let (x, y) = positions[participant];
        match c {
            '>' => positions[participant] = (x + 1, y),
            '<' => positions[participant] = (x - 1, y),
            '^' => positions[participant] = (x, y + 1),
            'v' => positions[participant] = (x, y - 1),
            _ => eprintln!("Invalid character read"),
        }
        visited_houses.insert(positions[participant]);
        participant = (participant + 1) % number_participants;
    }
    visited_houses
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn number_houses_visited_solo() {
        let movements_1 = "^>v<";
        let movements_2 = ">";
        let movements_3 = "^v^v^v^v^v";

        assert_eq!(calculate_visited_houses(movements_1, 1).len(), 4);
        assert_eq!(calculate_visited_houses(movements_2, 1).len(), 2);
        assert_eq!(calculate_visited_houses(movements_3, 1).len(), 2);
    }

    #[test]
    fn number_houses_visited_pair() {
        let movements_1 = "^v";
        let movements_2 = "^>v<";
        let movements_3 = "^v^v^v^v^v";

        assert_eq!(calculate_visited_houses(movements_1, 2).len(), 3);
        assert_eq!(calculate_visited_houses(movements_2, 2).len(), 3);
        assert_eq!(calculate_visited_houses(movements_3, 2).len(), 11);
    }
}
