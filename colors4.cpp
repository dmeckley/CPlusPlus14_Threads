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


    // Here's one way to use lambdas: We can make a lamda that has a few 
    // arguments, so that we can reuse it.

    auto blinker =
        [&](sf::Color & color, auto interval)
        {
            sf::Color original = color;

            while (go)
            {
                std::this_thread::sleep_for(interval);
                color = sf::Color::Black;
                std::this_thread::sleep_for(interval);
                color = original;
            }
        };

    // Just like in colors3.cpp, these arguments are copied when we create
    // the thread object, so we have to use std::ref for reference arguments.

    std::thread t1{blinker, std::ref(colors[0]), std::chrono::milliseconds(400)};
    std::thread t2{blinker, std::ref(colors[1]), std::chrono::milliseconds(300)};




    // Here's another way to use lambdas: We can have each lambda capture all
    // the arguments it needs. This way we don't have to worry about how to
    // pass arguments when we create the thread, but now we're stuck making
    // different lambdas for thread.

    auto blinker3 = 
        [&]()
        {
            sf::Color original = colors[2];
            auto interval = std::chrono::milliseconds(500);

            while (go)
            {
                std::this_thread::sleep_for(interval);
                colors[2] = sf::Color::Black;
                std::this_thread::sleep_for(interval);
                colors[2] = original;
            }
        };

    auto blinker4 = 
        [&]()
        {
            sf::Color original = colors[3];
            auto interval = std::chrono::milliseconds(600);

            while (go)
            {
                std::this_thread::sleep_for(interval);
                colors[3] = sf::Color::Black;
                std::this_thread::sleep_for(interval);
                colors[3] = original;
            }
        };



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

    go = false;

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
