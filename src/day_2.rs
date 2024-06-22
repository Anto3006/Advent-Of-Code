use aoc;
use std::{fs, process};

fn main() {
    let input = fs::read_to_string("inputs/day_2/part_1.txt").unwrap();
    let lines = aoc::split_string(&input, "\n");
    let mut total_area = 0;
    let mut total_ribbon_length = 0;
    for line in lines {
        if line.len() > 0 {
            let gift = Gift::build(&line).unwrap_or_else(|err| {
                eprintln!("Problem reading the file: {err}");
                process::exit(1);
            });
            let area = gift.calculate_paper_area();
            let ribbon_length = gift.calculate_ribbon_length();
            total_area += area;
            total_ribbon_length += ribbon_length;
        }
    }
    println!("The total area is {total_area} square feet");
    println!("The total ribbon length is {total_ribbon_length} feet");
}

struct Gift {
    length: u32,
    width: u32,
    height: u32,
}

impl Gift {
    fn new(length: u32, width: u32, height: u32) -> Gift {
        Gift {
            length,
            width,
            height,
        }
    }

    fn build(description: &str) -> Result<Gift, &'static str> {
        let dimensions = aoc::split_string(description, "x");
        if dimensions.len() == 3 {
            match (
                dimensions[0].parse(),
                dimensions[1].parse(),
                dimensions[2].parse(),
            ) {
                (Ok(length), Ok(width), Ok(height)) => Ok(Gift {
                    length,
                    width,
                    height,
                }),
                _ => Err("At least one dimension is incorrect"),
            }
        } else {
            Err("Wrong description format, it must be: lxwxh")
        }
    }

    fn calculate_paper_area(&self) -> u32 {
        let length_width = self.length * self.width;
        let width_height = self.width * self.height;
        let length_height = self.length * self.height;

        let min_side = {
            if length_width < width_height {
                if length_width < length_height {
                    length_width
                } else {
                    length_height
                }
            } else {
                if width_height < length_height {
                    width_height
                } else {
                    length_height
                }
            }
        };

        return 2 * length_width + 2 * width_height + 2 * length_height + min_side;
    }

    fn calculate_ribbon_length(&self) -> u32 {
        let bow_length = self.length * self.width * self.height;
        let wrapping_length = {
            if self.length < self.width {
                if self.width < self.height {
                    2 * (self.length + self.width)
                } else {
                    2 * (self.length + self.height)
                }
            } else {
                if self.length < self.height {
                    2 * (self.length + self.width)
                } else {
                    2 * (self.width + self.height)
                }
            }
        };
        bow_length + wrapping_length
    }
}
#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn area_calculation() {
        let gift = Gift::new(2, 3, 4);
        let area = gift.calculate_paper_area();
        assert_eq!(area, 58);
    }

    #[test]
    fn ribbon_length_calculation() {
        let gift = Gift::new(2, 3, 4);
        let length_ribbon = gift.calculate_ribbon_length();
        assert_eq!(length_ribbon, 34);
    }
}
