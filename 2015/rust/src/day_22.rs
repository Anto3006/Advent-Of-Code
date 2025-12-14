use std::{cmp::min, collections::VecDeque, u16};

const BASE_HERO_HP: u8 = 50;
const BASE_HER0_MP: u16 = 500;
const RECHARGE_MP_REGEN: u16 = 101;
const SHIELD_VALUE: u8 = 7;

const BASE_DK_HP: u8 = 51;
const BASE_DK_DAMAGE: u8 = 9;
const POISON_DAMAGE: u8 = 3;

const MAGIC_MISSILE_DAMAGE: u8 = 4;
const DRAIN_DAMAGE: u8 = 2;

#[derive(Copy, Clone)]
struct Hero {
    hit_points: u8,
    mana: u16,
    armor: u8,
    shield_count: u8,
    recharge_count: u8,
}

#[derive(Copy, Clone)]
struct DemonKing {
    hit_points: u8,
    damage: u8,
    poison_count: u8,
}

const SPELLS: [Spell; 5] = [
    Spell::MagicMissile,
    Spell::Drain,
    Spell::Shield,
    Spell::Poison,
    Spell::Recharge,
];

enum Spell {
    MagicMissile,
    Drain,
    Shield,
    Poison,
    Recharge,
}

#[derive(Copy, Clone)]
struct GameState {
    hero: Hero,
    demon_king: DemonKing,
    total_mp_usage: u16,
    is_hard_mode: bool,
}

fn main() {
    println!("This is day 22 of Advent of Code 2015 in rust!");
    let normal_mode_solution = find_best_solution(false);
    let hard_mode_solution = find_best_solution(true);
    println!("Part 1: {} MP", normal_mode_solution);
    println!("Part 2: {} MP", hard_mode_solution);
}

fn find_best_solution(is_hard_mode: bool) -> u16 {
    let initial_state = GameState::new(is_hard_mode);
    let mut best_solution = u16::MAX;
    let mut states = VecDeque::new();
    states.push_back(initial_state);
    while !states.is_empty() {
        let state = states.pop_front().unwrap();
        if state.check_game_end() {
            if state.demon_king.hit_points == 0 {
                if state.total_mp_usage < best_solution {
                    best_solution = state.total_mp_usage;
                }
            }
        } else {
            state.add_next_game_states(&mut states, best_solution);
        }
    }
    best_solution
}

impl Spell {
    fn get_mp_cost(&self) -> u16 {
        match self {
            Self::MagicMissile => 53,
            Self::Drain => 73,
            Self::Shield => 113,
            Self::Poison => 173,
            Self::Recharge => 229,
        }
    }
}

impl Hero {
    fn new() -> Self {
        Self {
            hit_points: BASE_HERO_HP,
            mana: BASE_HER0_MP,
            shield_count: 0,
            recharge_count: 0,
            armor: 0,
        }
    }

    fn turn_upkeep(&mut self) {
        if self.shield_count > 0 {
            self.shield_count -= 1;
            if self.shield_count == 0 {
                self.armor = 0;
            }
        }
        if self.recharge_count > 0 {
            self.mana += RECHARGE_MP_REGEN;
            self.recharge_count -= 1;
        }
    }

    fn apply_shield(&mut self) {
        self.shield_count = 6;
        self.armor = SHIELD_VALUE;
    }

    fn apply_recharge(&mut self) {
        self.recharge_count = 5;
    }

    fn take_damage(&mut self, damage: u8) {
        let effective_damage = if damage > self.armor {
            damage - self.armor
        } else {
            1
        };
        self.hit_points = self.hit_points.saturating_sub(effective_damage);
    }
}

impl DemonKing {
    fn new() -> Self {
        Self {
            hit_points: BASE_DK_HP,
            poison_count: 0,
            damage: BASE_DK_DAMAGE,
        }
    }

    fn turn_upkeep(&mut self) {
        if self.poison_count > 0 {
            self.take_damage(POISON_DAMAGE);
            self.poison_count -= 1;
        }
    }

    fn take_damage(&mut self, damage: u8) {
        self.hit_points = self.hit_points.saturating_sub(damage);
    }

    fn apply_poison(&mut self) {
        self.poison_count = 6;
    }
}

impl GameState {
    fn new(is_hard_mode: bool) -> Self {
        Self {
            hero: Hero::new(),
            demon_king: DemonKing::new(),
            total_mp_usage: 0,
            is_hard_mode,
        }
    }

    fn check_game_end(&self) -> bool {
        if self.hero.hit_points == 0 {
            return true;
        } else if self.demon_king.hit_points == 0 {
            return true;
        } else {
            return false;
        }
    }

    fn add_next_game_states(&self, queue: &mut VecDeque<GameState>, best_solution: u16) {
        for spell in SPELLS {
            let next_state = self.get_next_game_state_using_spell(spell);
            if let Some(next_state) = next_state {
                if next_state.total_mp_usage < best_solution {
                    queue.push_back(next_state);
                }
            }
        }
    }

    fn get_next_game_state_using_spell(&self, spell: Spell) -> Option<Self> {
        let mut next_state = self.clone();
        next_state.hero.turn_upkeep();
        if next_state.is_hard_mode {
            next_state.hero.take_damage(1);
        }
        next_state.demon_king.turn_upkeep();
        let spell_cost = spell.get_mp_cost();
        if next_state.check_game_end() {
            return Some(next_state);
        }
        if spell_cost > next_state.hero.mana {
            return None;
        }
        next_state.hero.mana -= spell_cost;
        next_state.total_mp_usage += spell_cost;
        match spell {
            Spell::MagicMissile => next_state.demon_king.take_damage(MAGIC_MISSILE_DAMAGE),
            Spell::Drain => {
                next_state.demon_king.take_damage(DRAIN_DAMAGE);
                next_state.hero.hit_points += DRAIN_DAMAGE;
            }
            Spell::Shield => {
                if next_state.hero.shield_count > 0 {
                    return None;
                } else {
                    next_state.hero.apply_shield();
                }
            }
            Spell::Poison => {
                if next_state.demon_king.poison_count > 0 {
                    return None;
                } else {
                    next_state.demon_king.apply_poison();
                }
            }
            Spell::Recharge => {
                if next_state.hero.recharge_count > 0 {
                    return None;
                } else {
                    next_state.hero.apply_recharge();
                }
            }
        }
        next_state.hero.turn_upkeep();
        next_state.demon_king.turn_upkeep();
        if next_state.check_game_end() {
            return Some(next_state);
        }
        next_state.hero.take_damage(next_state.demon_king.damage);
        Some(next_state)
    }
}
