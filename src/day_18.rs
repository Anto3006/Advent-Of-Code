use std::fs;

fn main() {
    let lights_input = fs::read_to_string("inputs/day_18/input.txt").unwrap();
    let mut light_grid = LightGrid::create_from_text_input(&lights_input, None);
    let corners = vec![
        Position::new(0, 0),
        Position::new(0, light_grid.width - 1),
        Position::new(light_grid.length - 1, 0),
        Position::new(light_grid.length - 1, light_grid.width - 1),
    ];
    let mut light_grid_fixed_corners =
        LightGrid::create_from_text_input(&lights_input, Some(corners));
    for _ in 1..=100 {
        light_grid.calculate_next_generation();
        light_grid_fixed_corners.calculate_next_generation();
    }
    let light_count = light_grid.count_lights_on();
    let light_count_fixed_corners = light_grid_fixed_corners.count_lights_on();
    println!("The number of lights on after 100 generations is {light_count}");
    println!(
        "The number of lights on after 100 generations for part 2 is {light_count_fixed_corners}"
    );
    println!("This is day 18 of Advent of Code 2015");
}

struct Position {
    row: usize,
    col: usize,
}

impl Position {
    fn new(row: usize, col: usize) -> Self {
        Self { row, col }
    }
}

struct LightGrid {
    length: usize,
    width: usize,
    lights: Vec<bool>,
    permanent_light: Option<Vec<Position>>,
}

impl LightGrid {
    fn new(length: usize, width: usize, permanent_light: Option<Vec<Position>>) -> Self {
        let mut lights = Vec::with_capacity(length * width);
        for index in 0..length * width {
            lights[index] = false;
        }
        if let Some(permanent_lights) = &permanent_light {
            for position in permanent_lights {
                let index = position.row * width + position.col;
                if index < length * width {
                    lights[index] = true;
                }
            }
        }
        LightGrid {
            length,
            width,
            lights,
            permanent_light,
        }
    }

    fn create_from_text_input(text: &str, permanent_light: Option<Vec<Position>>) -> Self {
        let length = text.lines().count();
        let width = text.lines().next().unwrap_or("").len();
        let mut lights = Vec::with_capacity(length * width);
        let mut index = 0;
        for line in text.lines() {
            for light in line.chars() {
                if index < length * width {
                    match light {
                        '.' => lights.push(false),
                        '#' => lights.push(true),
                        _ => {
                            lights.push(false);
                            eprintln!("Found incorrect light value");
                        }
                    }
                    index += 1;
                } else {
                    eprintln!("Light out of bounds");
                }
            }
        }
        if let Some(permanent_lights) = &permanent_light {
            for position in permanent_lights {
                let index = position.row * width + position.col;
                if index < length * width {
                    lights[index] = true;
                }
            }
        }
        Self {
            length,
            width,
            lights,
            permanent_light,
        }
    }

    fn calculate_next_generation(&mut self) {
        let mut next_lights = Vec::with_capacity(self.length * self.width);
        for index in 0..self.length * self.width {
            let row = index / self.width;
            let col = index % self.width;
            let mut is_permanent = false;
            if let Some(permanent_lights) = &self.permanent_light {
                for permanent_position in permanent_lights {
                    if row == permanent_position.row && col == permanent_position.col {
                        is_permanent = true;
                        break;
                    }
                }
            }
            if !is_permanent {
                let on_neighbors = self.calculate_on_neighbors(index);
                match self.lights[index] {
                    true => next_lights.push(on_neighbors == 2 || on_neighbors == 3),
                    false => next_lights.push(on_neighbors == 3),
                }
            } else {
                next_lights.push(true);
            }
        }
        self.lights = next_lights;
    }

    fn calculate_on_neighbors(&self, index: usize) -> u8 {
        let neighbors_positions = self.calculate_neighbors_positions(index);
        let mut on_count = 0;
        for neighbor_position in neighbors_positions {
            if self.lights[neighbor_position] {
                on_count += 1;
            }
        }
        on_count
    }

    fn calculate_neighbors_positions(&self, index: usize) -> Vec<usize> {
        let mut neighbors_positions = Vec::with_capacity(9);
        let row = index / self.width;
        let col = index % self.width;
        for drow in -1..=1 {
            for dcol in -1..=1 {
                match (row, col, drow, dcol) {
                    (0, _, -1, _) => (),
                    (_, 0, _, -1) => (),
                    (_, _, 0, 0) => (),
                    (_, _, _, _) => {
                        if (drow != 1 || row != self.length - 1)
                            && (dcol != 1 || col != self.width - 1)
                        {
                            let n_row = if drow < 0 {
                                row - (-drow as usize)
                            } else {
                                row + (drow as usize)
                            };
                            let n_col = if dcol < 0 {
                                col - (-dcol as usize)
                            } else {
                                col + (dcol as usize)
                            };
                            let n_index = n_row * self.width + n_col;
                            neighbors_positions.push(n_index);
                        }
                    }
                }
            }
        }
        neighbors_positions
    }

    fn count_lights_on(&self) -> u32 {
        let mut count = 0;
        for light in &self.lights {
            if *light {
                count += 1;
            }
        }
        count
    }
}
