use aoc::split_string;
use std::fs;

fn main() {
    let text = fs::read_to_string("inputs/day_14/input.txt").unwrap();
    let reindeers = parse_reindeers(&text);
    println!("{:?}", reindeers);
    let race_time = 2503.0;
    let max_dist = reindeers
        .iter()
        .map(|r| calculate_distance(r, race_time))
        .fold(f64::NEG_INFINITY, f64::max);
    println!("The maximum distance is: {max_dist}");
}

#[derive(Debug)]
struct Reindeer {
    name: String,
    speed: f64,
    moving_time: f64,
    resting_time: f64,
}

fn calculate_distance(reindeer: &Reindeer, race_time: f64) -> f64 {
    let mut distance_traveled = 0.0;
    let mut time_passed = 0.0;
    while time_passed < race_time {
        if time_passed + reindeer.moving_time <= race_time {
            distance_traveled += reindeer.speed * reindeer.moving_time;
            time_passed += reindeer.moving_time + reindeer.resting_time;
        } else {
            let time_available = race_time - time_passed;
            distance_traveled += reindeer.speed * time_available;
            time_passed += race_time - time_passed;
        }
    }
    distance_traveled
}

fn parse_reindeers(text: &str) -> Vec<Reindeer> {
    let mut reindeers = Vec::new();
    for line in text.lines() {
        let line_data = split_string(line, " ");
        let name = line_data[0].clone();
        let speed = line_data[3].parse().unwrap();
        let moving_time = line_data[6].parse().unwrap();
        let resting_time = line_data[13].parse().unwrap();
        let reindeer = Reindeer {
            name,
            speed,
            moving_time,
            resting_time,
        };
        reindeers.push(reindeer);
    }
    reindeers
}
