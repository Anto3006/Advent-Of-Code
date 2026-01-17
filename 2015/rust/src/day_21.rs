use aoc;
use std::{cmp::max, fs, u16};

#[derive(Copy, Clone, Debug)]
enum ItemType {
    Weapon,
    Armor,
    Ring,
}

#[derive(Debug)]
struct Item {
    item_type: ItemType,
    cost: u16,
    damage: u8,
    armor: u8,
}

struct Inventory {
    weapons: Vec<Item>,
    armors: Vec<Item>,
    rings: Vec<Item>,
}

fn main() {
    let inventory_data = fs::read_to_string("inputs/day_21/inventory.txt").unwrap();
    let inventory = Inventory::parse_from_file(&inventory_data);
    let min_cost = find_cheapest_combination_to_win(&inventory, 100, 109, 8, 2);
    let max_cost = find_most_expensive_combination_to_lose(&inventory, 100, 109, 8, 2);
    println!("This is day 21 of Advent of Code 2015 in rust");
    println!("Part 1: {} gold", min_cost);
    println!("Part 2: {} gold", max_cost);
}

fn find_cheapest_combination_to_win(
    inventory: &Inventory,
    hero_hp: u8,
    boss_hp: u8,
    boss_damage: u8,
    boss_armor: u8,
) -> u16 {
    let mut best_combination = u16::MAX;
    for weapon in &inventory.weapons {
        for armor in &inventory.armors {
            for idx_ring_1 in 0..inventory.rings.len() {
                let ring_1 = &inventory.rings[idx_ring_1];
                for idx_ring_2 in (idx_ring_1 + 1)..inventory.rings.len() {
                    let ring_2 = &inventory.rings[idx_ring_2];
                    if is_victory(
                        hero_hp,
                        boss_hp,
                        boss_damage,
                        boss_armor,
                        weapon,
                        Some(armor),
                        Some(ring_1),
                        Some(ring_2),
                    ) {
                        let cost = weapon.cost + armor.cost + ring_1.cost + ring_2.cost;
                        if cost < best_combination {
                            best_combination = cost;
                        }
                    }
                }
                if is_victory(
                    hero_hp,
                    boss_hp,
                    boss_damage,
                    boss_armor,
                    weapon,
                    Some(armor),
                    Some(&ring_1),
                    None,
                ) {
                    let cost = weapon.cost + armor.cost + ring_1.cost;
                    if cost < best_combination {
                        best_combination = cost;
                    }
                }
            }
            if is_victory(
                hero_hp,
                boss_hp,
                boss_damage,
                boss_armor,
                weapon,
                Some(armor),
                None,
                None,
            ) {
                let cost = weapon.cost + armor.cost;
                if cost < best_combination {
                    best_combination = cost;
                }
            }
        }
        for idx_ring_1 in 0..inventory.rings.len() {
            let ring_1 = &inventory.rings[idx_ring_1];
            for idx_ring_2 in (idx_ring_1 + 1)..inventory.rings.len() {
                let ring_2 = &inventory.rings[idx_ring_2];
                if is_victory(
                    hero_hp,
                    boss_hp,
                    boss_damage,
                    boss_armor,
                    weapon,
                    None,
                    Some(ring_1),
                    Some(ring_2),
                ) {
                    let cost = weapon.cost + ring_1.cost + ring_2.cost;
                    if cost < best_combination {
                        best_combination = cost;
                    }
                }
            }
            if is_victory(
                hero_hp,
                boss_hp,
                boss_damage,
                boss_armor,
                weapon,
                None,
                Some(&ring_1),
                None,
            ) {
                let cost = weapon.cost + ring_1.cost;
                if cost < best_combination {
                    best_combination = cost;
                }
            }
        }
        if is_victory(
            hero_hp,
            boss_hp,
            boss_damage,
            boss_armor,
            weapon,
            None,
            None,
            None,
        ) {
            let cost = weapon.cost;
            if cost < best_combination {
                best_combination = cost;
            }
        }
    }
    best_combination
}

fn find_most_expensive_combination_to_lose(
    inventory: &Inventory,
    hero_hp: u8,
    boss_hp: u8,
    boss_damage: u8,
    boss_armor: u8,
) -> u16 {
    let mut best_combination = 0;
    for weapon in &inventory.weapons {
        for armor in &inventory.armors {
            for idx_ring_1 in 0..inventory.rings.len() {
                let ring_1 = &inventory.rings[idx_ring_1];
                for idx_ring_2 in (idx_ring_1 + 1)..inventory.rings.len() {
                    let ring_2 = &inventory.rings[idx_ring_2];
                    if !is_victory(
                        hero_hp,
                        boss_hp,
                        boss_damage,
                        boss_armor,
                        weapon,
                        Some(armor),
                        Some(ring_1),
                        Some(ring_2),
                    ) {
                        let cost = weapon.cost + armor.cost + ring_1.cost + ring_2.cost;
                        if cost > best_combination {
                            best_combination = cost;
                        }
                    }
                }
                if !is_victory(
                    hero_hp,
                    boss_hp,
                    boss_damage,
                    boss_armor,
                    weapon,
                    Some(armor),
                    Some(&ring_1),
                    None,
                ) {
                    let cost = weapon.cost + armor.cost + ring_1.cost;
                    if cost > best_combination {
                        best_combination = cost;
                    }
                }
            }
            if !is_victory(
                hero_hp,
                boss_hp,
                boss_damage,
                boss_armor,
                weapon,
                Some(armor),
                None,
                None,
            ) {
                let cost = weapon.cost + armor.cost;
                if cost > best_combination {
                    best_combination = cost;
                }
            }
        }
        for idx_ring_1 in 0..inventory.rings.len() {
            let ring_1 = &inventory.rings[idx_ring_1];
            for idx_ring_2 in (idx_ring_1 + 1)..inventory.rings.len() {
                let ring_2 = &inventory.rings[idx_ring_2];
                if !is_victory(
                    hero_hp,
                    boss_hp,
                    boss_damage,
                    boss_armor,
                    weapon,
                    None,
                    Some(ring_1),
                    Some(ring_2),
                ) {
                    let cost = weapon.cost + ring_1.cost + ring_2.cost;
                    if cost > best_combination {
                        best_combination = cost;
                    }
                }
            }
            if !is_victory(
                hero_hp,
                boss_hp,
                boss_damage,
                boss_armor,
                weapon,
                None,
                Some(&ring_1),
                None,
            ) {
                let cost = weapon.cost + ring_1.cost;
                if cost > best_combination {
                    best_combination = cost;
                }
            }
        }
        if !is_victory(
            hero_hp,
            boss_hp,
            boss_damage,
            boss_armor,
            weapon,
            None,
            None,
            None,
        ) {
            let cost = weapon.cost;
            if cost > best_combination {
                best_combination = cost;
            }
        }
    }
    best_combination
}

fn is_victory(
    hero_hp: u8,
    boss_hp: u8,
    boss_damage: u8,
    boss_armor: u8,
    weapon: &Item,
    armor: Option<&Item>,
    ring_1: Option<&Item>,
    ring_2: Option<&Item>,
) -> bool {
    let mut total_damage = weapon.damage;
    let mut total_armor = weapon.armor;
    if let Some(armor) = armor {
        total_damage += armor.damage;
        total_armor += armor.armor;
    }
    if let Some(ring_1) = ring_1 {
        total_damage += ring_1.damage;
        total_armor += ring_1.armor;
    }
    if let Some(ring_2) = ring_2 {
        total_damage += ring_2.damage;
        total_armor += ring_2.armor;
    }
    let effective_attack = max(1, total_damage.saturating_sub(boss_armor));
    let effective_damage_taken = max(1, boss_damage.saturating_sub(total_armor));
    let turns_needed = if boss_hp % effective_attack == 0 {
        boss_hp / effective_attack
    } else {
        boss_hp / effective_attack + 1
    };
    effective_damage_taken.saturating_mul(turns_needed - 1) < hero_hp
}

impl Inventory {
    fn parse_from_file(inventory_data: &String) -> Self {
        let mut weapons = Vec::new();
        let mut armors = Vec::new();
        let mut rings = Vec::new();
        let mut reading_item_type = ItemType::Weapon;
        let mut have_read_header = false;
        for line in inventory_data.lines() {
            if line.len() == 0 {
                reading_item_type = match reading_item_type {
                    ItemType::Weapon => ItemType::Armor,
                    ItemType::Armor => ItemType::Ring,
                    ItemType::Ring => ItemType::Weapon,
                };
                have_read_header = false;
                continue;
            }
            if !have_read_header {
                have_read_header = true;
            } else {
                let parts: Vec<&str> = line.split_whitespace().collect();
                let cost: u16;
                let damage: u8;
                let armor: u8;
                match reading_item_type {
                    ItemType::Ring => {
                        cost = str::parse(parts[2]).unwrap();
                        damage = str::parse(parts[3]).unwrap();
                        armor = str::parse(parts[4]).unwrap();
                    }
                    _ => {
                        cost = str::parse(parts[1]).unwrap();
                        damage = str::parse(parts[2]).unwrap();
                        armor = str::parse(parts[3]).unwrap();
                    }
                }
                let item = Item {
                    item_type: reading_item_type,
                    cost,
                    armor,
                    damage,
                };
                match reading_item_type {
                    ItemType::Weapon => weapons.push(item),
                    ItemType::Armor => armors.push(item),
                    ItemType::Ring => rings.push(item),
                }
            }
        }
        Self {
            weapons,
            armors,
            rings,
        }
    }
}
