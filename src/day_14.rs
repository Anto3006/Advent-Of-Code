use aoc::split_string;
use std::fs;

fn main() {
    let text = fs::read_to_string("inputs/day_14/input.txt").unwrap();
    let mut reindeers = parse_reindeers(&text);
    println!("{:?}", reindeers);
    let race_time = 2503;
    let max_dist = reindeers
        .iter()
        .map(|r| calculate_distance(r, race_time))
        .fold(0, u64::max);
    let points = calculate_points(&mut reindeers, race_time);
    let max_points = points.iter().max().unwrap();
    println!("The maximum distance is: {max_dist}");
    println!("The winning reindeer has: {max_points} points");
}

#[derive(Debug)]
struct Reindeer {
    name: String,
    speed: u64,
    moving_time: u64,
    resting_time: u64,
    is_moving: bool,
    remaining_time_in_state: u64,
    distance: u64,
}

fn calculate_distance(reindeer: &Reindeer, race_time: u64) -> u64 {
    let mut distance_traveled = 0;
    let mut time_passed = 0;
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
            is_moving: true,
            remaining_time_in_state: moving_time,
            distance: 0,
        };
        reindeers.push(reindeer);
    }
    reindeers
}

fn calculate_points(reindeers: &mut Vec<Reindeer>, race_time: u64) -> Vec<u64> {
    let mut points: Vec<u64> = Vec::new();
    for _ in 0..reindeers.len() {
        points.push(0);
    }
    let mut distance_first_position = 0;
    for second in 0..race_time {
        for (index, reindeer) in reindeers.iter_mut().enumerate() {
            if reindeer.is_moving {
                reindeer.distance += reindeer.speed;
            }
            reindeer.remaining_time_in_state -= 1;
            if reindeer.remaining_time_in_state == 0 {
                reindeer.is_moving = !reindeer.is_moving;
                if reindeer.is_moving {
                    reindeer.remaining_time_in_state = reindeer.moving_time;
                } else {
                    reindeer.remaining_time_in_state = reindeer.resting_time;
                }
            }
            if reindeer.distance > distance_first_position {
                distance_first_position = reindeer.distance;
            }
        }
        for (index, reindeer) in reindeers.iter().enumerate() {
            if reindeer.distance == distance_first_position {
                points[index] += 1;
            }
        }
    }
    points
}

#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn test_most_points() {
        let comet = Reindeer {
            name: String::from("Comet"),
            speed: 14,
            moving_time: 10,
            resting_time: 127,
            is_moving: true,
            remaining_time_in_state: 10,
            distance: 0,
        };

        let dancer = Reindeer {
            name: String::from("Dancer"),
            speed: 16,
            moving_time: 11,
            resting_time: 162,
            is_moving: true,
            remaining_time_in_state: 11,
            distance: 0,
        };
        let mut reindeers: Vec<Reindeer> = Vec::new();
        reindeers.push(comet);
        reindeers.push(dancer);
        let points = calculate_points(&mut reindeers, 1000);
        assert_eq!(points[0], 312);
        assert_eq!(points[1], 689);
    }
}
