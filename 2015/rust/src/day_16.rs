use aoc::{self, split_string};
use std::fs;

fn main() {
    let base_aunt = AuntInformation {
        children: Some(3),
        cats: Some(7),
        samoyeds: Some(2),
        pomeranians: Some(3),
        akitas: Some(0),
        vizslas: Some(0),
        goldfish: Some(5),
        trees: Some(3),
        cars: Some(2),
        perfumes: Some(1),
    };
    let input = fs::read_to_string("inputs/day_16/input.txt").unwrap();
    let aunt_lines = split_string(&input, "\n");
    for (line_index, line) in aunt_lines.iter().enumerate() {
        let aunt_information = parse_aunt_information(&line);
        let aunt_number = line_index + 1;
        if is_aunt_possible(&base_aunt, &aunt_information, false) {
            println!("Aunt {aunt_number} is possible for part 1");
        }
        if is_aunt_possible(&base_aunt, &aunt_information, true) {
            println!("Aunt {aunt_number} is possible for part 2");
        }
    }
    println!("This is day 16 of Advent of Code 2015");
}

#[derive(Copy, Clone)]
struct AuntInformation {
    children: Option<u8>,
    cats: Option<u8>,
    samoyeds: Option<u8>,
    pomeranians: Option<u8>,
    akitas: Option<u8>,
    vizslas: Option<u8>,
    goldfish: Option<u8>,
    trees: Option<u8>,
    cars: Option<u8>,
    perfumes: Option<u8>,
}

fn parse_aunt_information(aunt_information: &str) -> AuntInformation {
    let mut parsed_aunt_information = AuntInformation {
        children: None,
        cats: None,
        samoyeds: None,
        pomeranians: None,
        akitas: None,
        vizslas: None,
        goldfish: None,
        trees: None,
        cars: None,
        perfumes: None,
    };
    let (_, aunt_data) = aunt_information.split_once(": ").unwrap();
    let individual_aunt_data = aoc::split_string(aunt_data, ", ");
    for parameter_data in individual_aunt_data {
        let parameter_parts = aoc::split_string(&parameter_data, ": ");
        let parameter_name = parameter_parts[0].as_str();
        let parameter_value = Some(parameter_parts[1].parse().unwrap());
        match parameter_name {
            "children" => parsed_aunt_information.children = parameter_value,
            "cats" => parsed_aunt_information.cats = parameter_value,
            "samoyeds" => parsed_aunt_information.samoyeds = parameter_value,
            "pomeranians" => parsed_aunt_information.pomeranians = parameter_value,
            "akitas" => parsed_aunt_information.akitas = parameter_value,
            "vizslas" => parsed_aunt_information.vizslas = parameter_value,
            "goldfish" => parsed_aunt_information.goldfish = parameter_value,
            "trees" => parsed_aunt_information.trees = parameter_value,
            "cars" => parsed_aunt_information.cars = parameter_value,
            "perfumes" => parsed_aunt_information.perfumes = parameter_value,
            _ => eprintln!("Unknown parameter {parameter_name}"),
        }
    }
    parsed_aunt_information
}

fn is_aunt_possible(
    base_aunt: &AuntInformation,
    other_aunt: &AuntInformation,
    is_part_2: bool,
) -> bool {
    let mut possible = true;
    match (base_aunt.children, other_aunt.children) {
        (Some(base_children), Some(other_children)) => {
            possible = (possible) && (base_children == other_children)
        }
        (_, _) => (),
    }
    if is_part_2 {
        match (base_aunt.cats, other_aunt.cats) {
            (Some(base_cats), Some(other_cats)) => {
                possible = (possible) && (base_cats < other_cats)
            }
            (_, _) => (),
        }
    } else {
        match (base_aunt.cats, other_aunt.cats) {
            (Some(base_cats), Some(other_cats)) => {
                possible = (possible) && (base_cats == other_cats)
            }
            (_, _) => (),
        }
    }
    match (base_aunt.samoyeds, other_aunt.samoyeds) {
        (Some(base_samoyeds), Some(other_samoyeds)) => {
            possible = (possible) && (base_samoyeds == other_samoyeds)
        }
        (_, _) => (),
    }
    if is_part_2 {
        match (base_aunt.pomeranians, other_aunt.pomeranians) {
            (Some(base_pomeranians), Some(other_pomeranians)) => {
                possible = (possible) && (base_pomeranians > other_pomeranians)
            }
            (_, _) => (),
        }
    } else {
        match (base_aunt.pomeranians, other_aunt.pomeranians) {
            (Some(base_pomeranians), Some(other_pomeranians)) => {
                possible = (possible) && (base_pomeranians == other_pomeranians)
            }
            (_, _) => (),
        }
    }
    match (base_aunt.akitas, other_aunt.akitas) {
        (Some(base_akitas), Some(other_akitas)) => {
            possible = (possible) && (base_akitas == other_akitas)
        }
        (_, _) => (),
    }
    match (base_aunt.vizslas, other_aunt.vizslas) {
        (Some(base_vizslas), Some(other_vizslas)) => {
            possible = (possible) && (base_vizslas == other_vizslas)
        }
        (_, _) => (),
    }
    if is_part_2 {
        match (base_aunt.goldfish, other_aunt.goldfish) {
            (Some(base_goldfish), Some(other_goldfish)) => {
                possible = (possible) && (base_goldfish > other_goldfish)
            }
            (_, _) => (),
        }
    } else {
        match (base_aunt.goldfish, other_aunt.goldfish) {
            (Some(base_goldfish), Some(other_goldfish)) => {
                possible = (possible) && (base_goldfish == other_goldfish)
            }
            (_, _) => (),
        }
    }
    if is_part_2 {
        match (base_aunt.trees, other_aunt.trees) {
            (Some(base_trees), Some(other_trees)) => {
                possible = (possible) && (base_trees < other_trees)
            }
            (_, _) => (),
        }
    } else {
        match (base_aunt.trees, other_aunt.trees) {
            (Some(base_trees), Some(other_trees)) => {
                possible = (possible) && (base_trees == other_trees)
            }
            (_, _) => (),
        }
    }
    match (base_aunt.cars, other_aunt.cars) {
        (Some(base_cars), Some(other_cars)) => possible = (possible) && (base_cars == other_cars),
        (_, _) => (),
    }
    match (base_aunt.perfumes, other_aunt.perfumes) {
        (Some(base_perfumes), Some(other_perfumes)) => {
            possible = (possible) && (base_perfumes == other_perfumes)
        }
        (_, _) => (),
    }
    possible
}
