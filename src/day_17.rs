use std::{cmp::min, fs, u32::MAX};

fn main() {
    let container_information =
        fs::read_to_string("inputs/day_17/input.txt").expect("cannot open file");
    let containers = parse_containers(&container_information);
    let target = 150;
    let number_combinations_free_choice = calculate_combinations_to_fill(&containers, target, None);
    let minimum_container_count = calculate_minimum_container_count_to_fill(&containers, target);
    let number_combinations_min_count =
        calculate_combinations_to_fill(&containers, target, Some(minimum_container_count));
    println!("The number of combinations is {number_combinations_free_choice}");
    println!("The minimum number of containers to use is {minimum_container_count}");
    println!("The number of combinations that use the minimum number of containers is {number_combinations_min_count}");
    println!("This is day 17 of 2015's Advent of Code");
}

fn parse_containers(container_information: &str) -> Vec<u32> {
    let mut containers = Vec::new();
    for line in container_information.lines() {
        let container_volume = line.parse().unwrap();
        containers.push(container_volume);
    }
    containers
}

fn calculate_combinations_to_fill(
    containers: &[u32],
    target: u32,
    container_target: Option<u32>,
) -> u32 {
    return _aux_calculate_combinations_to_fill(&containers, target, container_target, 0);
}

fn _aux_calculate_combinations_to_fill(
    containers: &[u32],
    target: u32,
    container_target: Option<u32>,
    current_container_used: u32,
) -> u32 {
    if target == 0 {
        match container_target {
            None => return 1,
            Some(container_target) => {
                if container_target == current_container_used {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
    } else if containers.len() == 0 {
        return 0;
    } else if containers.len() == 1 {
        match container_target {
            None => {
                if target == containers[0] {
                    return 1;
                } else {
                    return 0;
                }
            }
            Some(container_target) => {
                if target == containers[0] && container_target == current_container_used + 1 {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
    } else {
        if containers[0] > target {
            return _aux_calculate_combinations_to_fill(
                &containers[1..],
                target,
                container_target,
                current_container_used,
            );
        } else {
            return _aux_calculate_combinations_to_fill(
                &containers[1..],
                target - containers[0],
                container_target,
                current_container_used + 1,
            ) + _aux_calculate_combinations_to_fill(
                &containers[1..],
                target,
                container_target,
                current_container_used,
            );
        }
    }
}

fn calculate_minimum_container_count_to_fill(containers: &[u32], target: u32) -> u32 {
    return _aux_calculate_minimum_container_count_to_fill(&containers, target, 0);
}

fn _aux_calculate_minimum_container_count_to_fill(
    containers: &[u32],
    target: u32,
    current_container_used: u32,
) -> u32 {
    if target == 0 {
        return current_container_used;
    }
    if containers.len() == 0 {
        return MAX;
    } else if containers.len() == 1 {
        if containers[0] == target {
            return 1 + current_container_used;
        } else {
            return MAX;
        }
    } else {
        if containers[0] > target {
            return _aux_calculate_minimum_container_count_to_fill(
                &containers[1..],
                target,
                current_container_used,
            );
        } else {
            return min(
                _aux_calculate_minimum_container_count_to_fill(
                    &containers[1..],
                    target - containers[0],
                    current_container_used + 1,
                ),
                _aux_calculate_minimum_container_count_to_fill(
                    &containers[1..],
                    target,
                    current_container_used,
                ),
            );
        }
    }
}

#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn test_calculation_combinations() {
        let containers = [20, 15, 10, 5, 5];
        let target = 25;
        let number_combiations = calculate_combinations_to_fill(&containers, target);
        assert_eq!(number_combiations, 4);
    }

    #[test]
    fn test_minimum_container_count() {
        let containers = [20, 15, 10, 5, 5];
        let target = 25;
        let minimum_count = calculate_minimum_container_count_to_fill(&containers, target, 0);
        assert_eq!(minimum_count, 2);
    }
}
