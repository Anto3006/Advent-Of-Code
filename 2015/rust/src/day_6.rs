use aoc;
use std::fs;
use std::num::ParseIntError;
use std::usize;

fn main() {
    let input = fs::read_to_string("inputs/day_6/part_1.txt").unwrap();
    let mut lights = LightGrid::new(1000, 1000);
    for line in input.lines() {
        let action = Action::build(line).unwrap();
        lights.apply_action(action);
    }
    let mut count: u64 = 0;
    for light in lights.lights {
        count += light as u64;
    }
    println!("{count}");
}

#[derive(Debug, PartialEq, Eq)]
struct Position {
    x: i64,
    y: i64,
}

impl Position {
    fn build(s: &str) -> Result<Self, ParseIntError> {
        let coordinates = aoc::split_string(s, ",");
        let x: i64 = coordinates[0].parse()?;
        let y: i64 = coordinates[1].parse()?;
        Ok(Self { x, y })
    }
}

struct PositionIterator {
    initial_position: Position,
    final_position: Position,
    current_position: Position,
    direction: (i8, i8),
}

impl PositionIterator {
    fn new(initial_position: &Position, final_position: &Position) -> Self {
        let x_dir: i8 = if initial_position.x <= final_position.x {
            if initial_position.x == final_position.x {
                0
            } else {
                1
            }
        } else {
            -1
        };
        let y_dir: i8 = if initial_position.y <= final_position.y {
            if initial_position.y == final_position.y {
                0
            } else {
                1
            }
        } else {
            -1
        };
        PositionIterator {
            initial_position: Position {
                ..*initial_position
            },
            final_position: Position { ..*final_position },
            current_position: Position {
                x: initial_position.x - x_dir as i64,
                ..*initial_position
            },
            direction: (x_dir, y_dir),
        }
    }
}

impl Iterator for PositionIterator {
    type Item = Position;
    fn next(&mut self) -> Option<Self::Item> {
        if self.current_position.x < self.final_position.x {
            self.current_position.x += self.direction.0 as i64;
        } else if self.current_position.y < self.final_position.y {
            self.current_position.x = self.initial_position.x;
            self.current_position.y += self.direction.1 as i64;
        } else {
            return None;
        }
        Some(Position {
            ..self.current_position
        })
    }
}

enum ActionType {
    TurnOn,
    TurnOff,
    Toggle,
}

struct Action {
    action: ActionType,
    initial_position: Position,
    final_position: Position,
}

impl Action {
    fn build(s: &str) -> Option<Self> {
        let action_parts = aoc::split_string(s, " ");
        match action_parts[0].as_str() {
            "turn" => {
                let initial_position = Position::build(&action_parts[2]).unwrap();
                let final_position = Position::build(&action_parts[4]).unwrap();
                match action_parts[1].as_str() {
                    "on" => {
                        return Some(Action {
                            action: ActionType::TurnOn,
                            initial_position,
                            final_position,
                        })
                    }
                    "off" => {
                        return Some(Action {
                            action: ActionType::TurnOff,
                            initial_position,
                            final_position,
                        })
                    }
                    _ => None,
                }
            }
            "toggle" => {
                let initial_position = Position::build(&action_parts[1]).unwrap();
                let final_position = Position::build(&action_parts[3]).unwrap();
                return Some(Action {
                    action: ActionType::Toggle,
                    initial_position,
                    final_position,
                });
            }
            _ => return None,
        }
    }
}

struct LightGrid {
    lights: Vec<u16>,
    height: usize,
    width: usize,
}

impl LightGrid {
    fn new(height: usize, width: usize) -> Self {
        LightGrid {
            lights: vec![0; height * width],
            height,
            width,
        }
    }

    fn pos_to_index(&self, position: Position) -> Option<usize> {
        let index = (position.y as usize) * self.width + (position.x as usize);
        if index >= self.lights.len() {
            return None;
        }
        Some(index)
    }

    fn increase_light(&mut self, position: Position, value: u16) {
        let index = self.pos_to_index(position);
        match index {
            Some(index) => self.lights[index] += value,
            None => eprintln!("Invalid position"),
        }
    }

    fn decrease_light(&mut self, position: Position, value: u16) {
        let index = self.pos_to_index(position);
        match index {
            Some(index) => {
                if self.lights[index] >= value {
                    self.lights[index] -= value;
                } else {
                    self.lights[index] = 0;
                }
            }
            None => eprintln!("Invalid position"),
        }
    }

    fn apply_action(&mut self, action: Action) {
        let positions = PositionIterator::new(&action.initial_position, &action.final_position);
        match action.action {
            ActionType::TurnOn => {
                for position in positions {
                    self.increase_light(position, 1);
                }
            }
            ActionType::TurnOff => {
                for position in positions {
                    self.decrease_light(position, 1);
                }
            }
            ActionType::Toggle => {
                for position in positions {
                    self.increase_light(position, 2);
                }
            }
        }
    }
}

#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn test_iterator() {
        let initial_position = Position { x: 0, y: 0 };
        let final_position = Position { x: 10, y: 15 };
        let mut current_position = Position { x: 0, y: 0 };
        let iter = PositionIterator::new(&initial_position, &final_position);
        for position in iter {
            println!("{:#?},{:#?}", position, current_position);
            assert_eq!(position, current_position);
            if current_position.x < final_position.x {
                current_position.x += 1;
            } else if current_position.y < final_position.y {
                current_position.x = initial_position.x;
                current_position.y += 1;
            }
        }
    }
}
