use aoc;
use std::collections::{HashMap, HashSet};
use std::fs;

struct Molecule {
    atom_sequence: Vec<u8>,
    atom_association: HashMap<String, u8>,
    original_sequence: String,
}

struct ReplacementRules {
    rules: HashMap<String, ReplacementRule>,
}

struct ReplacementRule {
    before: String,
    after: Vec<String>,
    associated_before: u8,
    associated_after: Vec<Vec<u8>>,
}

fn main() {
    let input_data = fs::read_to_string("inputs/day_19/input.txt").unwrap();
    let mut replacement_rules = ReplacementRules::new();
    let mut is_reading_rules = true;
    let mut molecule = Molecule::new();
    for line in input_data.lines() {
        if !line.is_empty() {
            if is_reading_rules {
                replacement_rules.add_rule_from_line(line);
            } else {
                molecule.read_from_text(line);
            }
        }
        if line.is_empty() {
            is_reading_rules = false;
        }
    }
    replacement_rules.translate_rules(&mut molecule.atom_association);
    let new_possible_molecules = molecule.all_single_replacements(&replacement_rules);
    println!("This is day 19 of advent of code 2015 in rust");
    println!(
        "Part 1: {} distinct molecules",
        new_possible_molecules.len()
    );
    let steps = count_steps_to_molecule(&molecule);
    println!("Part 2: {} steps", steps);
}

fn count_steps_to_molecule(mol: &Molecule) -> usize {
    let rn_number = mol.atom_association.get("Rn").unwrap();
    let ar_number = mol.atom_association.get("Ar").unwrap();
    let y_number = mol.atom_association.get("Y").unwrap();

    let mut rn_count = 0;
    let mut ar_count = 0;
    let mut y_count = 0;

    for atom_number in mol.atom_sequence.iter() {
        if atom_number == rn_number {
            rn_count += 1;
        } else if atom_number == ar_number {
            ar_count += 1;
        } else if atom_number == y_number {
            y_count += 1;
        }
    }
    let steps = mol.atom_sequence.len() - rn_count - ar_count - 2 * y_count - 1;
    steps
}

fn translate_atoms(atom_text: &str, atom_association: &mut HashMap<String, u8>) -> Vec<u8> {
    let mut next_atom_value = match atom_association.is_empty() {
        false => *atom_association.values().max().unwrap() + 1,
        true => 1,
    };
    let mut translation = Vec::new();
    let mut atom_buffer = String::new();
    for character in atom_text.chars() {
        if character.is_uppercase() && !atom_buffer.is_empty() {
            if !atom_association.contains_key(&atom_buffer) {
                atom_association.insert(atom_buffer.clone(), next_atom_value);
                next_atom_value += 1;
            }
            let atom_found = atom_association.get(&atom_buffer).unwrap();
            translation.push(*atom_found);
            atom_buffer.clear();
        }
        atom_buffer.push(character);
    }
    if !atom_buffer.is_empty() {
        if !atom_association.contains_key(&atom_buffer) {
            atom_association.insert(atom_buffer.clone(), next_atom_value);
        }
        let atom_found = atom_association.get(&atom_buffer).unwrap();
        translation.push(*atom_found);
        atom_buffer.clear();
    }
    translation
}

impl ReplacementRule {
    fn new(before: &str, after: &str) -> Self {
        Self {
            before: before.to_string(),
            after: vec![after.to_string()],
            associated_before: 0,
            associated_after: Vec::new(),
        }
    }

    fn add_possible_after(&mut self, after: &str) {
        self.after.push(after.to_string());
    }

    fn translate_rule(&mut self, association: &mut HashMap<String, u8>) {
        self.associated_before = translate_atoms(&self.before, association)[0];
        for possible_after in self.after.iter() {
            self.associated_after
                .push(translate_atoms(possible_after, association));
        }
    }
}

impl ReplacementRules {
    fn new() -> Self {
        Self {
            rules: HashMap::new(),
        }
    }

    fn add_rule_from_line(&mut self, line: &str) {
        let rule_parts = aoc::split_string(line, " => ");
        let before = &rule_parts[0];
        let after = &rule_parts[1];
        self.rules
            .entry(before.clone())
            .and_modify(|rule| rule.add_possible_after(after))
            .or_insert(ReplacementRule::new(before, after));
    }

    fn translate_rules(&mut self, association: &mut HashMap<String, u8>) {
        for (_, rule) in self.rules.iter_mut() {
            rule.translate_rule(association);
        }
    }
}

impl Molecule {
    fn new() -> Self {
        Self {
            atom_sequence: Vec::new(),
            atom_association: HashMap::new(),
            original_sequence: String::new(),
        }
    }

    fn read_from_text(&mut self, text: &str) {
        self.atom_sequence.clear();
        self.atom_association.clear();
        self.original_sequence = text.to_string();
        self.atom_sequence = translate_atoms(text, &mut self.atom_association);
    }

    fn possible_single_replacements(
        &self,
        before: u8,
        after: &Vec<u8>,
        found_states: &mut HashSet<Vec<u8>>,
    ) -> Vec<Vec<u8>> {
        let mut possibilities = Vec::new();
        for (pos, atom) in self.atom_sequence.iter().enumerate() {
            if *atom == before {
                let mut new_possibility = self.atom_sequence.clone();
                for (pos_after, atom_after) in after.iter().enumerate() {
                    if pos_after == 0 {
                        new_possibility[pos] = after[0];
                    } else {
                        new_possibility.insert(pos + pos_after, *atom_after);
                    }
                }
                if !found_states.contains(&new_possibility) {
                    found_states.insert(new_possibility.clone());
                    possibilities.push(new_possibility);
                }
            }
        }
        possibilities
    }

    fn all_single_replacements(&self, replacement_rules: &ReplacementRules) -> Vec<Vec<u8>> {
        let mut possibilites = Vec::new();
        let mut found_states: HashSet<Vec<u8>> = HashSet::new();
        for (_, rule) in replacement_rules.rules.iter() {
            let before = rule.associated_before;
            for possible_after in rule.associated_after.iter() {
                possibilites.extend(self.possible_single_replacements(
                    before,
                    possible_after,
                    &mut found_states,
                ));
            }
        }
        possibilites
    }
}
