#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <vector>
#include <iostream>

#define cell_size 10
#define cells_x 51
#define cells_y 51
#define window_x cell_size*cells_x
#define window_y cell_size*cells_y
#define condition i == 25 || j == 25

using namespace std;

class cell
{
private:
	int x, y;
	sf::RectangleShape shape;
	bool border;

public:
	cell()
	{
		border = false;
		shape.setOutlineColor(sf::Color::White);
		x = 0;
		y = 0;
		shape.setFillColor(sf::Color::Black);
		shape.setSize(sf::Vector2f(cell_size, cell_size));
		shape.setPosition(x * cell_size, y * cell_size);
	}

	cell(int x, int y)
	{
		border = false;
		shape.setOutlineColor(sf::Color::White);
		this->x = x;
		this->y = y;
		shape.setFillColor(sf::Color::Black);
		shape.setSize(sf::Vector2f(cell_size, cell_size));
		shape.setPosition(x * cell_size, y * cell_size);
	}

	cell(int x, int y, sf::Color color)
	{
		border = false;
		shape.setOutlineColor(sf::Color::White);
		this->x = x;
		this->y = y;
		shape.setFillColor(color);
		shape.setSize(sf::Vector2f(cell_size, cell_size));
		shape.setPosition(x * cell_size, y * cell_size);
	}

	void changeColor(sf::Color color)
	{
		shape.setFillColor(color);
	}

	void setPos(int x, int y)		//x and y are in grid positions, not pixels
	{
		this->x = x;
		this->y = y;
		shape.setPosition(x * cell_size, y * cell_size);
		cout << shape.getGlobalBounds().left << " " << shape.getGlobalBounds().left << endl;
	}

	void render(sf::RenderWindow &window)
	{
		window.draw(shape);
	}

	sf::Color color()
	{
		return this->shape.getFillColor();
	}

	void toggleBorder()
	{
		if (border)
		{
			shape.setOutlineThickness(0);
		}
		else
		{
			shape.setOutlineThickness(1);
		}
		border ^= 1;
	}
};

class table
{
private:
	vector<vector<cell> > cells;
	bool simIsRunning;
	bool grid;

public:
	table()
	{
		grid = false;
		for (int i = 0; i < cells_x; i++)
		{
			vector<cell> blank;
			cells.push_back(blank);
			for (int j = 0; j < cells_y; j++)
			{
				cells[i].push_back(cell(i, j));
				int live = rand() % 2;
				if (live)
				{
					cells[i][j].changeColor(sf::Color::Yellow);
				}
			}
		}
		simIsRunning = true;
	}

	table(bool hack)	//Don't do this ever
	{
		grid = false;
		for (int i = 0; i < cells_x; i++)
		{
			vector<cell> blank;
			cells.push_back(blank);
			for (int j = 0; j < cells_y; j++)
			{
				cells[i].push_back(cell(i, j));
				if (condition)
				{
					cells[i][j].changeColor(sf::Color::Yellow);
				}
			}
		}
		simIsRunning = true;
	}

	void update()
	{
		vector<vector<cell> > new_cells;
		for (int i = 0; i < cells.size(); i++)
		{
			vector<cell> blank;
			new_cells.push_back(blank);
			for (int j = 0; j < cells[i].size(); j++)
			{
				new_cells[i].push_back(cell(i, j, cells[i][j].color()));
				int live_neighbors = 0;
				for (int x = -1; x < 2; x++)
				{
					for (int y = -1; y < 2; y++)
					{
						if ((i + x >= 0 && i + x < cells.size()) && (j + y >= 0 && j + y < cells[i].size()) && (x != 0 || y!= 0))
						{
							if (cells[i + x][j + y].color() == sf::Color::Yellow || cells[i + x][j + y].color() == sf::Color::Blue)
							{
								live_neighbors++;
							}
						}
					}
				}
				if (cells[i][j].color() == sf::Color::Yellow || cells[i][j].color() == sf::Color::Blue)	//Live
				{
					if (live_neighbors != 2 && live_neighbors != 3)
					{
						new_cells[i][j] = cell(i, j, sf::Color::Black);
					}
					else
					{
						new_cells[i][j] = cell(i, j, sf::Color::Blue);
					}
				}
				else if (cells[i][j].color() == sf::Color::Black)	//Dead
				{
					if (live_neighbors == 3)
					{
						new_cells[i][j] = cell(i, j, sf::Color::Yellow);
					}
				}
			}
		}
		cells = new_cells;
	}

	void render(sf::RenderWindow& window)
	{
		for (int i = 0; i < cells.size(); i++)
		{
			for (int j = 0; j < cells[i].size(); j++)
			{
				cells[i][j].render(window);
			}
		}
	}

	bool simRunning()
	{
		return simIsRunning;
	}

	void pause()
	{
		simIsRunning = false;
	}

	void unpause()
	{
		simIsRunning = true;
	}

	sf::Vector2i mouseOver(sf::RenderWindow &window)
	{
		sf::Vector2i toReturn;
		sf::Vector2i mouse = sf::Mouse::getPosition(window);
		int x = mouse.x / cell_size;
		int y = mouse.y / cell_size;
		if (x >= 0 && y >= 0)
		{
			toReturn.x = mouse.x / cell_size;
			toReturn.y = mouse.y / cell_size;
			//cout << "mouse over " << toReturn.x << ", " << toReturn.y << endl;
		}
		else
		{
			toReturn.x = -1;
			toReturn.y = -1;
		}
		return toReturn;
	}

	sf::Vector2i mouseClick(sf::RenderWindow &window)
	{
		sf::Vector2i mouseHover = mouseOver(window);
		sf::Vector2i minusOne;
		minusOne.x = -1;
		minusOne.y = -1;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			return mouseHover;
		}
		return minusOne;
	}

	void setCellColor(int x, int y, sf::Color color)
	{
		cells.at(x).at(y).changeColor(color);
	}

	sf::Color cellColor(int x, int y)
	{
		return cells.at(x).at(y).color();
	}

	void toggleGrid()
	{
		for (int i = 0; i < cells_x; i++)
		{
			for (int j = 0; j < cells_y; j++)
			{
				cells[i][j].toggleBorder();
			}
		}
	}
};

int main()
{
	cout << "Welcome to Conway's Game of Life!" << endl;
	cout << "---------------------------------" << endl;
	cout << "Controls-" << endl;
	cout << "p - Pause and unpause the simulation" << endl;
	cout << "c - Clear the board" << endl;
	cout << "e - Enter and exit mouse edit mode - click on a cell to toggle life and death" << endl;
	cout << "Escape - exit the program" << endl;
	srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(window_x, window_y), "Conway's Game of Life");
	window.setFramerateLimit(144);
	table my_table(true);
	bool edit = false;
	sf::Vector2i minusOne;
	minusOne.x = -1;
	minusOne.y = -1;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::P && !edit)
				{
					if (my_table.simRunning())
					{
						my_table.pause();
					}
					else
					{
						my_table.unpause();
					}
				}
				else if (event.key.code == sf::Keyboard::E)
				{
					my_table.toggleGrid();
					edit ^= 1;
				}
			}
        }
        window.clear();
		my_table.render(window);
        window.display();
		if (edit)
		{
			sf::Vector2i coord = my_table.mouseClick(window);
			if (coord != minusOne)
			{
				if (my_table.cellColor(coord.x, coord.y) == sf::Color::Black)
				{
					my_table.setCellColor(coord.x, coord.y, sf::Color::Yellow);
				}
				else if (my_table.cellColor(coord.x, coord.y) == sf::Color::Blue || my_table.cellColor(coord.x, coord.y) == sf::Color::Yellow)
				{
					my_table.setCellColor(coord.x, coord.y, sf::Color::Yellow);
				}
			}
		}
		if (my_table.simRunning() && !edit) my_table.update();
    }

    return 0;
}