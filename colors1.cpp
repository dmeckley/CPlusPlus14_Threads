#include <thread>
#include <vector>
#include <chrono>
#include <SFML/Graphics.hpp>


const int WIDTH = 640;
const int HEIGHT = 480;


sf::Color colors[4] =
    {sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow};

bool running = true;

void blinker1()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        colors[0] = sf::Color::Black;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        colors[0] = sf::Color::Red;
    }
}

void blinker2()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        colors[1] = sf::Color::Black;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        colors[1] = sf::Color::Blue;
    }
}

void blinker3()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        colors[2] = sf::Color::Black;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        colors[2] = sf::Color::Green;
    }
}

void blinker4()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
        colors[3] = sf::Color::Black;
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
        colors[3] = sf::Color::Yellow;
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML");

    sf::CircleShape circles[4];
    circles[0].setPosition(100, 100);
    circles[1].setPosition(400, 100);
    circles[2].setPosition(100, 300);
    circles[3].setPosition(400, 300);
    for (auto & c: circles)
        c.setRadius(50);

    std::thread t1{blinker1};
    std::thread t2{blinker2};
    std::thread t3{blinker3};
    std::thread t4{blinker4};

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

    running = false;

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
