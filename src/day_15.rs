use std::{
    cmp::max,
    ops::{Add, Mul},
};

fn main() {
    println!("This is advent of code 2015 day 15");
}

struct Ingredient {
    name: String,
    capacity: i32,
    durabitiliy: i32,
    flavor: i32,
    texture: i32,
    calories: i32,
}

struct Cookie {
    capacity: i32,
    durabitiliy: i32,
    flavor: i32,
    texture: i32,
    calories: i32,
}

impl Mul<i32> for Ingredient {
    type Output = Cookie;

    fn mul(self, rhs: i32) -> Self::Output {
        Cookie {
            capacity: self.capacity * rhs,
            durabitiliy: self.durabitiliy * rhs,
            flavor: self.durabitiliy * rhs,
            texture: self.texture * rhs,
            calories: self.calories * rhs,
        }
    }
}

impl Add<Cookie> for Cookie {
    type Output = Cookie;

    fn add(self, rhs: Cookie) -> Self::Output {
        Cookie {
            capacity: self.capacity + rhs.capacity,
            durabitiliy: self.durabitiliy + rhs.durabitiliy,
            flavor: self.flavor + rhs.flavor,
            texture: self.texture + rhs.texture,
            calories: self.calories + rhs.calories,
        }
    }
}

impl Cookie {
    fn score(self) -> u32 {
        let capacity_score: u32 = match u32::try_from(self.capacity) {
            Ok(u32) => u32,
            Err(_) => 0,
        };
        let durability_score: u32 = match u32::try_from(self.durabitiliy) {
            Ok(u32) => u32,
            Err(_) => 0,
        };
        let flavor_score: u32 = match u32::try_from(self.flavor) {
            Ok(u32) => u32,
            Err(_) => 0,
        };
        let texture_score: u32 = match u32::try_from(self.texture) {
            Ok(u32) => u32,
            Err(_) => 0,
        };
        capacity_score * durability_score * flavor_score * texture_score
    }
}

#[cfg(test)]
mod tests {
    use super::*;
}
