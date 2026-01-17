use std::fs;

fn main() {
    let input = fs::read_to_string("inputs/day_24/input.txt").unwrap();
    let weights = parse_weights(&input);
    let weight_sum: u64 = weights.iter().sum();
    let target = weight_sum / 4;
    for count in 1..7 {
        println!(
            "-------------------------------------{}----------------------------------",
            count
        );
        let best = calculate_best_group(&weights, target, count);
        println!("{:?}", best);
    }
    println!("This is day 24 of Advent of Code 2015 in rust!");
}

fn parse_weights(input: &str) -> Vec<u64> {
    let mut weights = Vec::new();
    for line in input.lines() {
        let weight = line.parse::<u64>().unwrap();
        weights.push(weight);
    }
    weights
}

fn calculate_quatum_entanglement(weights: &Vec<u64>, used: &Vec<bool>) -> u64 {
    if weights.len() != used.len() {
        return 0;
    }
    let mut quantum_entanglement = 1;
    for (pos, is_weight_used) in used.iter().enumerate() {
        if *is_weight_used {
            quantum_entanglement *= weights[pos];
        }
    }
    quantum_entanglement
}

fn calculate_best_group(weights: &Vec<u64>, target: u64, count: u64) -> Option<u64> {
    let mut used: Vec<bool> = Vec::new();
    for _ in 0..weights.len() {
        used.push(false);
    }
    calculate_best_group_aux(weights, target, &mut used, count, weights.len() - 1, None)
}

fn calculate_best_group_aux(
    weights: &Vec<u64>,
    target: u64,
    used: &mut Vec<bool>,
    count: u64,
    pos: usize,
    best_quatum_entanglement: Option<u64>,
) -> Option<u64> {
    if count * weights[pos] < target {
        return None;
    }
    if count == 0 {
        let result = calculate_quatum_entanglement(weights, used);
        println!("{}", result);
        return Some(result);
    }
    if pos < (count as usize) - 1 {
        return None;
    }
    if pos == 0 && weights[pos] != target {
        return None;
    }
    if count == 1 && weights[pos] == target {
        let result = calculate_quatum_entanglement(weights, used) * weights[pos];
        println!("{}", result);
        return Some(result);
    }
    let mut best = best_quatum_entanglement;
    let best_if_used;
    if weights[pos] <= target {
        used[pos] = true;
        let new_target = target - weights[pos];
        best_if_used = calculate_best_group_aux(
            weights,
            new_target,
            used,
            count - 1,
            pos - 1,
            best_quatum_entanglement,
        );
        if let Some(best_if_used) = best_if_used {
            if let Some(best_q) = best_quatum_entanglement {
                if best_if_used < best_q {
                    best = Some(best_if_used);
                }
            } else {
                best = Some(best_if_used);
            }
        }
    }
    used[pos] = false;
    let best_if_not_used = calculate_best_group_aux(weights, target, used, count, pos - 1, best);
    if let Some(best_if_not_used) = best_if_not_used {
        if let Some(best_found) = best {
            if best_if_not_used < best_found {
                best = Some(best_if_not_used);
            }
        } else {
            best = Some(best_if_not_used);
        }
    }
    best
}
