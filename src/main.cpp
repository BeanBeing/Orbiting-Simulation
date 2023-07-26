#include <SFML/Graphics.hpp>
#include <cmath>

// The source that has Gravitational pull
class GravitySource
{
   sf::Vector2f m_pos;
   float m_strength;
   sf::CircleShape m_cShape;

   /**--Member-Movement Variables--**/
   bool m_UpPressed;
   bool m_DownPressed;
   bool m_LeftPressed;
   bool m_RightPressed;

   float m_Speed{ 200 };
   /**--END OF MEMBER-MOVEMENT Variables--**/
   
public:
   GravitySource(float pos_x, float pos_y, float strength) : m_pos{pos_x, pos_y}, m_strength{strength}
   {
      m_cShape.setPosition(m_pos);
      m_cShape.setFillColor(sf::Color::Yellow);
      m_cShape.setRadius(12);
   }

   /**--SETTERS--**/
   void set_pos(float pos_x, float pos_y)
   {
      m_pos.x = pos_x;
      m_pos.y = pos_y;
   }

   void set_strength(float newStrength)
   {
      m_strength = newStrength;
   }
   /**--END OF SETTERS--**/

   /**--GETTERS--**/
   sf::Vector2f get_pos() const
   {
      return m_pos;
   }

   float get_strength() const
   {
      return m_strength;
   }
   /**--END OF GETTERS--**/

   /**----MOVEMENT----**/
   void moveUp()	{ m_UpPressed	 = true; }
   void moveDown()	{ m_DownPressed	 = true; }
   void moveLeft()	{ m_LeftPressed	 = true; }
   void moveRight()	{ m_RightPressed = true; }

   void stopUp()	{ m_UpPressed	 = false; }
   void stopDown()	{ m_DownPressed	 = false; }
   void stopLeft()	{ m_LeftPressed	 = false; }
   void stopRight()	{ m_RightPressed = false; }

   // Update each frame by the elapsedTime
   void update(float elapsedTime)
   {
      if(m_UpPressed)
      {
         m_pos.y -= m_Speed * elapsedTime;
      }

      if(m_DownPressed)
      {
         m_pos.y += m_Speed * elapsedTime;
      }

      if(m_LeftPressed)
      {
         m_pos.x -= m_Speed * elapsedTime;
      }

      if(m_RightPressed)
      {
         m_pos.x += m_Speed * elapsedTime;
      }

      // Update position each time the position is changed
      m_cShape.setPosition(m_pos);
   }
   /**----END OF MOVEMENT----**/

   void render(sf::RenderWindow &window)
   {
      window.draw(m_cShape);
   }
};

// The source that latches onto that gravitational pull
class Planet
{
   sf::Vector2f m_pos;		// Get position of Planet
   sf::Vector2f m_velocity;	// Rotating velocity of the Planet
   sf::CircleShape m_cShape;	// Shape of the Planet being pulled
   
public:
   Planet(float pos_x, float pos_y, float vel_x, float vel_y) : m_pos{pos_x, pos_y}, m_velocity{vel_x, vel_y}
   {
      m_cShape.setPosition(m_pos);
      m_cShape.setFillColor(sf::Color::Blue);
      m_cShape.setRadius(8);
   }

   // Manually change the color of a Planet
   void setShapeColor(sf::Color color)
   {
      m_cShape.setFillColor(color);
   }

   // Sets the shapePosition and Velocity. Required for resetting its state
   void setShapePos_vel(float pos_x, float pos_y, float vel_x, float vel_y)
   {
      m_pos.x = pos_x;
      m_pos.y = pos_y;
      m_velocity.x = vel_x;
      m_velocity.y = vel_y;
      m_cShape.setPosition(m_pos);
   }

   // Pythagorean theorem to determine the distance
   // between the GravitySource and the planet
   void update_physics(const GravitySource &obj)
   {
      float distance_x = obj.get_pos().x - m_pos.x;
      float distance_y = obj.get_pos().y - m_pos.y;

      // Gets the distance of x and y thanks to Pythagorean theorem
      // This is practically Pythagorean theorem formula stored into distance
      float distance = sqrt((distance_x * distance_x) + (distance_y * distance_y));

      // Any number multiply by a division of 1 divided by a number
      // is that number.
      // EX: 1/8 = 0.125 ; 5/8 = 0.125 * 5
      float inverse_distance = 1.f / distance;

      // Multiplication is faster than division
      float normalized_x = inverse_distance * distance_x;
      float normalized_y = inverse_distance * distance_y;

      // 1 / x * 1/x formula for inverse_square_law
      // inverse_distance provides mathematical equation of that formula
      float inverse_square_law = inverse_distance * inverse_distance;
      
      // If we take in the normalized value times the strength
      // Gravity is going to have the same pull no matter how far Planet is
      // This is not ideal in real situations...
      // Fixed by applying inverse-square-law
      float acceleration_x = normalized_x * obj.get_strength() * inverse_square_law;
      float acceleration_y = normalized_y * obj.get_strength() * inverse_square_law;

      m_velocity.x += acceleration_x;
      m_velocity.y += acceleration_y;

      m_pos.x += m_velocity.x;
      m_pos.y += m_velocity.y;

   }

   // Display the shape
   void render(sf::RenderWindow &window)
   {
      // Sets position of shape upon displayed
      m_cShape.setPosition(m_pos);
      window.draw(m_cShape);
   }

};


int main()
{
   sf::RenderWindow window(sf::VideoMode(800, 600), "Planet Simulation");

   GravitySource sun(400, 300, 1400);

   Planet planet1(400, 600, 2, 0);
   Planet planet2(450, 200, 3, 1.2);
   Planet planet3(400, 540, 2.4, 0);

   /**--Planets Independent Attribute--**/
   planet2.setShapeColor(sf::Color::Magenta);

   planet3.setShapeColor(sf::Color::White);
   /**--End Planet Independent Attribute--**/

   // Clock
   sf::Clock clock;
   // Framerate
   window.setFramerateLimit(60);

   while(window.isOpen())
   {
      sf::Event event;
      sf::Time dt = clock.restart();

      float dtAsSeconds = dt.asSeconds();
      while(window.pollEvent(event))
      {
         if(event.type == sf::Event::Closed)
         {
            window.close();
         }

         /**--------MOVEMENT--------**/

         if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
         {
            window.close();
         }

         if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
         {
            sun.moveUp();
         } else {
            sun.stopUp();
         }

         if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
         {
            sun.moveDown();
         } else {
            sun.stopDown();
         }

         if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
         {
            sun.moveLeft();
         } else {
            sun.stopLeft();
         }

         if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
         {
            sun.moveRight();
         } else {
            sun.stopRight();
         }

         /**--------RESET SCENE--------**/
         if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
         {
            sun.set_pos(400, 300);
            planet1.setShapePos_vel(400, 600, 2, 0);
            planet2.setShapePos_vel(450, 200, 3, 1.2);
            planet3.setShapePos_vel(400, 540, 2.4, 0);
         }
      }

      window.clear();
      sun.update(dtAsSeconds);
      planet1.update_physics(sun);
      planet2.update_physics(sun);
      planet3.update_physics(sun);

      /**--DRAW--**/

      sun.render(window);
      planet1.render(window);
      planet2.render(window);
      planet3.render(window);

      window.display();
   }
   
   return 0;
}
