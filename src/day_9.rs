use aoc;
use std::collections::HashMap;
use std::fs;

fn main() {
    println!("This is day 9 of advent of code 2015");
    let data = fs::read_to_string("inputs/day_9/input.txt").unwrap_or_default();
    let mut loc = Locations::default();
    loc.parse_locations(&data);
    let mut locations = loc.locations.clone();
    let permutations = generate_permutations(&mut locations);
    let mut best_cost: Option<u64> = None;
    let mut best_trip: Option<Vec<String>> = None;

    let mut worst_cost: Option<u64> = None;
    let mut worst_trip: Option<Vec<String>> = None;
    for trip in permutations {
        let cost = loc.evaluate_trip(&trip);
        match best_cost {
            None => {
                best_cost = Some(cost);
                best_trip = Some(trip.clone());
            }
            Some(current_best) => {
                if current_best > cost {
                    best_cost = Some(cost);
                    best_trip = Some(trip.clone());
                }
            }
        }
        match worst_cost {
            None => {
                worst_cost = Some(cost);
                worst_trip = Some(trip.clone());
            }
            Some(current_worst) => {
                if current_worst < cost {
                    worst_cost = Some(cost);
                    worst_trip = Some(trip.clone());
                }
            }
        }
    }
    println!(
        "The best trip is {:?}\nAnd has a cost of: {}",
        best_trip.unwrap_or_default(),
        best_cost.unwrap_or_default()
    );
    println!(
        "The worst trip is {:?}\nAnd has a cost of: {}",
        worst_trip.unwrap_or_default(),
        worst_cost.unwrap_or_default()
    );
}

fn generate_permutations(data: &mut Vec<String>) -> Vec<Vec<String>> {
    fn generate(k: usize, vector: &mut Vec<String>) -> Vec<Vec<String>> {
        if k == 1 {
            return vec![vector.clone()];
        } else {
            let mut first = generate(k - 1, vector);
            for i in 0..k - 1 {
                if k % 2 == 0 {
                    let temp = vector[i].clone();
                    vector[i] = vector[k - 1].clone();
                    vector[k - 1] = temp;
                } else {
                    let temp = vector[0].clone();
                    vector[0] = vector[k - 1].clone();
                    vector[k - 1] = temp;
                }
                first.extend(generate(k - 1, vector));
            }
            return first;
        }
    }
    generate(data.len(), data)
}

#[derive(Default)]
struct Locations {
    locations: Vec<String>,
    distances: HashMap<String, HashMap<String, u64>>,
}

impl Locations {
    fn parse_locations(&mut self, text: &str) {
        for line in text.lines() {
            let data = aoc::split_string(line, " = ");
            let locations = aoc::split_string(&data[0], " to ");
            let distance = data[1].parse().unwrap();
            if let None = self.distances.get(&locations[0]) {
                self.locations.push(locations[0].clone());
            }
            if let None = self.distances.get(&locations[1]) {
                self.locations.push(locations[1].clone());
            }
            *self
                .distances
                .entry(locations[0].clone())
                .or_default()
                .entry(locations[1].clone())
                .or_insert(0) = distance;
            *self
                .distances
                .entry(locations[1].clone())
                .or_default()
                .entry(locations[0].clone())
                .or_insert(0) = distance;
        }
    }

    fn evaluate_trip(&self, trip_order: &Vec<String>) -> u64 {
        let mut trip_cost = 0;
        for position in 0..trip_order.len() - 1 {
            let begin = &trip_order[position];
            let end = &trip_order[position + 1];
            trip_cost += self.distances.get(begin).unwrap().get(end).unwrap();
        }
        trip_cost
    }
}
