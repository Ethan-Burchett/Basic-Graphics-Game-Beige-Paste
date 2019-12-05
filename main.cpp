#include <SFML/Graphics.hpp>
//#include <Time.hpp>
#include "functions.h"
#include <random>
#include <time.h>
#include <iostream>
//#include <Color.hpp>

using namespace std;

int main()
{
	sf::Color color1(100, 100, 100, 255);
	srand((unsigned)(time(NULL)));
	
	sf::Clock clock;
	sf::Time time1 = clock.getElapsedTime();

	//////////////////////////////////////////////////////////////////
	//WINDOW
	//////////////////////////////////////////////////////////////////
	float boarder_width = 45;
	float b_pad = 50;
	float window_height = 1400;
	float window_width = 2000;

	sf::RenderWindow window(sf::VideoMode((int)window_width, (int)window_height), "beige paste", sf::Style::Close);
	window.setVerticalSyncEnabled(true);// helps with framerate

	window_height = 1400 - b_pad - b_pad;
	window_width = 2000 - b_pad - b_pad;
	
	sf::RectangleShape left(sf::Vector2f(boarder_width, window_height)); // left side
	sf::RectangleShape right(sf::Vector2f(boarder_width, window_height)); // right side
	sf::RectangleShape top(sf::Vector2f(window_width - b_pad, boarder_width)); // top side
	sf::RectangleShape bottom(sf::Vector2f(window_width - b_pad + boarder_width, boarder_width)); // top side

	right.setPosition(window_width, b_pad);
	left.setPosition(b_pad, b_pad);
	top.setPosition(b_pad, b_pad);
	bottom.setPosition(b_pad,window_height+b_pad);

	//////////////////////////////////////////////////////////////////
	// SHAPE ICON
	//////////////////////////////////////////////////////////////////
		int sides = 4; // sides of shape
		sf::CircleShape shape(40.f); // 30 is good
		shape.setPointCount(sides);
		shape.setRotation(45); // for square
		shape.setPosition((float)(window_width / 2), (float)((window_height / 2) - 100)); // puts shape in center of screen
		shape.setFillColor(color1);

		// movement speeds
		float upm = 0.0, downm = 0.0, leftm = 0.0, rightm = 0.0;
		float thrust = 0.2;
		float gravity = 0.0;
		float g_force = thrust*0.3;
		
		float rotate_speed = .35;
		int black_count = 0;
		int red_count = 0;
		int red_duration = 40;
		int colision_trigger = 0; // 0 for false 1 for true colision
		float radius = 40;
		int touched_blob = 0; // 0 for false 1 for true colision
	
	/////////////////////////////////////////////////////////////////
	// OBS 
    /////////////////////////////////////////////////////////////////
		float obs_move_speed = 3;
		
		float side_length = 70;
		float rand_side_length = rand() % 150 + 200;  // max size obs 350
		float amount_moved = 10000;         //rand() % 150 + 10;

		float obs_pos_x = rand() % (int)(window_width - 300) + 51;// -350; //+150; //- rand_side_length;
		float obs_pos_y = rand() % 50 + 250;//(int)(window_height - 300) + 51;// -350;// +150;// - rand_side_length*1.61803398875;

		sf::RectangleShape obs_rec(sf::Vector2f(side_length, rand_side_length*1.61803398875));
		obs_rec.setFillColor(sf::Color::White);
		obs_rec.setPosition(obs_pos_x, obs_pos_y);

		//horizontal bar
		sf::RectangleShape obs_rec_2(sf::Vector2f(((double)(rand() % 150 + 200)*1.61803398875), side_length));
		obs_rec_2.setFillColor(sf::Color::White);
		obs_rec_2.setPosition(rand() % 500+351, rand() % 100 +150);

		//2nd horizontal bar
		sf::RectangleShape obs_rec_3(sf::Vector2f(((double)(rand() % 150 + 100)*1.61803398875), side_length));
		obs_rec_3.setFillColor(sf::Color::White);
		obs_rec_3.setPosition(rand() % 500 + 351, rand() % 150 + 900);

		// for obs 1 vert
		int obs_count = 0;
		int obs_u_d = 0;// 0 down 1 up
						// for obs 2 horz
		int obs_count_2 = 0;
		int obs_u_d_2 = 0;// 0 down 1 up
						  // for obs 3 horz
		int obs_count_3 = 0;
		int obs_u_d_3 = 0;// 0 down 1 up

		int color_delay = 0;
		int change_color = 0; // 0 yellow, 1 red
		float obs_rotate = 0;

	///////////////////////////////////////////////////////////
	// BLOB MONSTER
	///////////////////////////////////////////////////////////
		// create an non-init shape
		sf::ConvexShape convex(10.f);
		

		convex.setFillColor(sf::Color::Magenta);
		convex.setPointCount(5);
		convex.setPosition(rand() % 500 + 100, rand() % 500 + 100);

		int blob_count = 0;
		float blob_up = 0, blob_down = 0, blob_left = 0, blob_right = 0;
		float blob_gravity = 0;
		float blob_thrust = 0.04;
		float blob_g_force = blob_thrust*0.3;
		int blob_direction = 0; //0 for down 1 for up 2 for left 3 for right 
		float blob_slowdown_rate = 0.9;
		int blob_scale = 10;
		int blob_radius = 2;
	//////////////////////////////////////////////////////////////////////
	// GREEN PILLS
	//////////////////////////////////////////////////////////////////////
		int pill_size = 30;
	sf::CircleShape pill(pill_size);
	pill.setFillColor(sf::Color::Green);
	//randomly generate pills on screen
	pill.setPosition(500, 500);
	float pill_shrink_rate = 0.0;




	int temp;
	//////////////////////////////////////////////////////////////////////
	// GAME LOOP
	//////////////////////////////////////////////////////////////////////
	//clear->draw->display
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event));
		{
			//////////////////////////////////////////////////////////////////////////////////
			//OBS MOVEMENT
			/////////////////////////////////////////////////////////////////////////////////
			{
				obs_count++;
				obs_count_2++;
				obs_count_3++;
				//////////////////////// obs 1
				if (obs_count > amount_moved)// duration for obstacle movement 
				{
					obs_count = 0;
					obs_u_d++;
				}
				if (obs_u_d % 2 == 1)// up
					obs_rec.move(0, -obs_move_speed*0.9);

				if (obs_u_d % 2 == 0)// down
					obs_rec.move(0, +obs_move_speed*0.9);
				obs_rec.rotate(obs_rotate);

				/////////////////////// obs 2
				if (obs_count_2 > amount_moved)// duration for obstacle movement 
				{
					obs_count_2 = 0;
					obs_u_d_2++;
				}
				if (obs_u_d_2 % 2 == 1)// up
					obs_rec_2.move(-obs_move_speed*0.8, 0);

				if (obs_u_d_2 % 2 == 0)// down
					obs_rec_2.move(+obs_move_speed*0.8, 0);
				obs_rec_2.rotate(obs_rotate);
				//////////////////////////////////////////////////////////

				/////////////////////// obs 3
				if (obs_count_3 > amount_moved)// duration for obstacle movement 
				{
					obs_count_3 = 0;
					obs_u_d_3++;
				}
				if (obs_u_d_3 % 2 == 1)// up
					obs_rec_3.move(-obs_move_speed, 0);

				if (obs_u_d_3 % 2 == 0)// down
					obs_rec_3.move(+obs_move_speed, 0);
				obs_rec_3.rotate(obs_rotate);
			}
			//////////////////////////////////////////////////////////
			//GREEN PILLS
			////////////////////////////////////////////////////////





			///////////////////////////////////////////////////////////////////////////
			//COLISION  
			////////////////////////////////////////////////////////////////////////////
			{
				sf::FloatRect shapeBound = shape.getGlobalBounds();  //  shape bound box

				sf::FloatRect obsBound = obs_rec.getGlobalBounds();  //    obs bound box

				sf::FloatRect obsBound_2 = obs_rec_2.getGlobalBounds();//  obs2 bound box

				sf::FloatRect obsBound_3 = obs_rec_3.getGlobalBounds();//  obs2 bound box

				sf::FloatRect leftBound = left.getGlobalBounds();    //   left bound box 

				sf::FloatRect rightBound = right.getGlobalBounds();  //  right bound box

				sf::FloatRect topBound = top.getGlobalBounds();		 //    top bound box

				sf::FloatRect bottomBound = bottom.getGlobalBounds();// bottom bound box

				sf::FloatRect blob_bound = convex.getGlobalBounds();// bottom bound box

				sf::FloatRect pill_bound = pill.getGlobalBounds();// bottom bound box

				if (pill_bound.intersects(shapeBound))
				{// randomly move pill
				// points ++
					pill_size++;
					pill.setPosition(rand() % (int)(window_width - 300) + 150, rand() % (int)(window_height - 300) + 150);
					pill.setRadius(pill_size);
					obs_move_speed += 0.2;
					
				}
				if (pill_bound.intersects(obsBound_3) || pill_bound.intersects(obsBound_2) || pill_bound.intersects(obsBound))
				{
					pill.move(rand() % 10 - 5, rand() % 10 - 5);

				}

				// if obs 1 touches 
				if (obsBound.intersects(obsBound_3) || obsBound.intersects(obsBound_2) || obsBound.intersects(leftBound) || obsBound.intersects(rightBound) || obsBound.intersects(topBound) || obsBound.intersects(bottomBound))
				{
					obs_u_d++;
				}

				// if obs 2 touches 
				if (obsBound_2.intersects(obsBound_3) || obsBound_2.intersects(obsBound) || obsBound_2.intersects(leftBound) || obsBound_2.intersects(rightBound) || obsBound_2.intersects(topBound) || obsBound_2.intersects(bottomBound))
				{
					obs_u_d_2++;
				}

				// if obs 3 touches 
				if (obsBound_3.intersects(obsBound) || obsBound_3.intersects(leftBound) || obsBound_3.intersects(rightBound) || obsBound_3.intersects(topBound) || obsBound_3.intersects(bottomBound))
				{
					obs_u_d_3++;
				}

				// if blob touches anything
				if (blob_bound.intersects(obsBound_3) || blob_bound.intersects(obsBound_2) || blob_bound.intersects(obsBound) || blob_bound.intersects(leftBound) || blob_bound.intersects(rightBound) || blob_bound.intersects(topBound) || blob_bound.intersects(bottomBound))
				{
					/// basic reflection physics
					// needs to slow down slightly
					blob_left = -blob_left*blob_slowdown_rate;
					blob_right = -blob_right*blob_slowdown_rate;
					blob_up = -blob_up*blob_slowdown_rate;
					blob_down = -blob_down*blob_slowdown_rate;
					//convex.move(rand() % 10 - 5, rand() % 10 - 5);
				}
				
				
				// if shape touches blob only
				if (shapeBound.intersects(blob_bound))
				{
					touched_blob = 1;

				}

				// if shape touches anything
				if (shapeBound.intersects(blob_bound) || shapeBound.intersects(obsBound_3) || shapeBound.intersects(obsBound_2) || shapeBound.intersects(obsBound) || shapeBound.intersects(leftBound) || shapeBound.intersects(rightBound) || shapeBound.intersects(topBound) || shapeBound.intersects(bottomBound)) // main colision part
				{

					colision_trigger = 1;

				}

				if (colision_trigger == 1)
				{
					red_count++;
					radius--;
					// do all this while red
					shape.setFillColor(sf::Color::Red);
					shape.setRadius(radius);
					//shape.move(rand() % 2 - 2, rand() % 2 - 2);
					downm = 0;
					upm = 0;
					leftm = 0;
					rightm = 0;
					obs_move_speed = 1;
					blob_scale+=0.1;
					
					if (red_count > red_duration) // stay red for duration
					{
						obs_move_speed = 3;
						radius = 40;
						shape.setRadius(radius);
					
						colision_trigger = 0;
						red_count = 0;
						shape.setFillColor(color1);
						shape.setPosition((float)(window_width / 2), (float)((window_height / 2) - 100));
						shape.setRotation(45);
					}

				}
				///////////////////////////////////////////////////////////////////
				//basic physics and movement
				///////////////////////////////////////////////////////////////////
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))// down
				{
					downm += thrust*0.8;
					//vertm -= thrust;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))// up
				{
					upm -= thrust;
					//vertm += thrust;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))// left
				{
					leftm -= thrust;
					shape.rotate(-rotate_speed);
					//horz -= thrust;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))// right
				{
					rightm += thrust;
					shape.rotate(rotate_speed);
					//vertm += thrust;
				}
				//gravity += g_force;

			
				//shape.move(leftm - rightm, upm - downm);
				shape.move(leftm + rightm, upm + downm + gravity);
	//////////////////////////////////////////////////////////////////////////
	//BLOB MOVEMENT
	//////////////////////////////////////////////////////////////////////////
				blob_count++; // acts as timer for movement decisions
				convex.rotate(0);
				if (blob_count > rand() % 1000 + 50)// determines how long it takes to change direction// should be random
				{
					blob_direction = rand() % 4; // picks a direction 
					blob_count = 0;

					convex.setPoint(0, sf::Vector2f(0, 0));
					convex.setPoint(1, sf::Vector2f(rand() % 80 + blob_scale, rand() % 80 + blob_scale));
					convex.setPoint(2, sf::Vector2f(rand() % 80 + blob_scale, rand() % 80 + blob_scale));
					convex.setPoint(3, sf::Vector2f(rand() % 80 + blob_scale, rand() % 80 + blob_scale));
					convex.setPoint(4, sf::Vector2f(0, rand() % 80 + blob_scale));
				}
				if (blob_direction == 0)// down
				{
					blob_down += blob_thrust;
				}
				if (blob_direction == 1)// up
				{
					blob_up -= blob_thrust;
				}
				if (blob_direction == 2)// left
				{
					blob_left -= blob_thrust;
				}
				if (blob_direction == 3)// right
				{
					blob_right += blob_thrust;
				}

				//blob_gravity += blob_g_force;
				convex.move(blob_left + blob_right, blob_up + blob_down + blob_gravity);
			}
				
				
			////////////////////////////////////////////////////////////////////////////////////
			//SYSTEM    QUIT     DRAW
			////////////////////////////////////////////////////////////////////////////////////
			if (event.type == sf::Event::Closed) { window.close(); }
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)){window.close();}

			//if (colision_trigger == 0)
			//{
				window.clear();
			//}

				window.draw(shape);
				window.draw(left);
				window.draw(top);
				window.draw(right);
				window.draw(bottom);
				window.draw(convex);
				window.draw(obs_rec);
				window.draw(obs_rec_2);
				window.draw(obs_rec_3);
				window.draw(pill);

				window.display();
				}
		}
	}

	return 0;
}