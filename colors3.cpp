#include <thread>
#include <vector>
#include <chrono>
#include <SFML/Graphics.hpp>


const int WIDTH = 640;
const int HEIGHT = 480;



void blink(sf::Color & color, bool & running, std::chrono::milliseconds interval)
{
    sf::Color original = color;

    while (running)
    {
        std::this_thread::sleep_for(interval);
        color = sf::Color::Black;
        std::this_thread::sleep_for(interval);
        color = original;
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML");

    sf::Color colors[4] = {sf::Color::Red, sf::Color::Blue, sf::Color::Green,
        sf::Color::Yellow};

    bool go = true;

    sf::CircleShape circles[4];
    circles[0].setPosition(100, 100);
    circles[1].setPosition(400, 100);
    circles[2].setPosition(100, 300);
    circles[3].setPosition(400, 300);
    for (auto & c: circles)
        c.setRadius(50);

    std::thread t1{blink, std::ref(colors[0]), std::ref(go), std::chrono::milliseconds(400)};
    std::thread t2{blink, std::ref(colors[1]), std::ref(go), std::chrono::milliseconds(300)};
    std::thread t3{blink, std::ref(colors[2]), std::ref(go), std::chrono::milliseconds(500)};
    std::thread t4{blink, std::ref(colors[3]), std::ref(go), std::chrono::milliseconds(600)};

    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear(sf::Color::Black);

        for (int i = 0; i < 4; i++)
        {
            circles[i].setFillColor(colors[i]);
            window.draw(circles[i]);
        }

        window.display();    
    }

    go = false;

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
