#include "app.h"
#include "Body.h"
#include "utils.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <unistd.h>


#define DT 0.01
#define STIFFNESS 100
#define AVERAGE_FRAMES 50
#define N_BODIES 2


using std::vector;


void updateFps(sf::Text &fps_text, double& start, double end, int& frameCounter) {
    if (frameCounter++ < AVERAGE_FRAMES) return;
    frameCounter = 0;
    double fps = AVERAGE_FRAMES / (end - start);
    if (std::abs(fps) < 1000) fps_text.setString(string_format("FPS: %.2lf", fps));
    else fps_text.setString("FPS: 999.99");
    start = end;
}


void fill(vector<Body> &bodies, u_int width, u_int height) {
    srand(time(nullptr));
    vector<sf::Color> colors = {
            sf::Color::Red,
            sf::Color::Blue,
            sf::Color::Cyan,
            sf::Color::Green,
            sf::Color::Magenta,
            sf::Color::White,
            sf::Color::Yellow,
    };
    u_int bound = std::min(width, height) / 4;
    u_int cx = width / 2;
    u_int cy = height / 2;

    double px = 0, py = 0;

    for (int i = 0; i < N_BODIES; ++i) {
        double vx = (double) (rand() % bound) / 100;
        double vy = (double) (rand() % bound) / 100;
        double mass = 500 + rand() % 500;
        if (i == N_BODIES - 1) {
            vx = -px / mass;
            vy = -py / mass;
        }
        px += vx * mass;
        py += vy * mass;

        bodies.emplace_back(
                mass,
                cx + (bound - rand() % (2 * bound)),
                cy + (bound - rand() % (2 * bound)),
                vx,
                vy,
                mass / 50 + rand() % 20,
                colors[rand() % colors.size()]
        );
    }
}


std::vector<double> getDotState(std::vector<double> state, vector<Body> &bodies) {
    u_long shift = state.size() / 2;
    std::vector<double> result(state.size());
    double x_i, y_i, x_j, y_j, dx, dy, dis, ax, ay;

    for (u_long i = 0; i < shift; ++i) result[i] = state[shift + i];
    for (u_long i = 0; i < bodies.size(); ++i) {
        ax = 0;
        ay = 0;
        for (u_long j = 0; j < bodies.size(); ++j) {
            if (i == j) continue;
            x_i = state[2 * i];
            y_i = state[2 * i + 1];
            x_j = state[2 * j];
            y_j = state[2 * j + 1];
            dx = x_j - x_i;
            dy = y_j - y_i;
            dis = sqrt(dx * dx + dy * dy);
            ax += bodies[j].mass * dx / (dis * dis * dis);
            ay += bodies[j].mass * dy / (dis * dis * dis);
            if (dis < bodies[i].r + bodies[j].r) {
                ax -= (bodies[i].r + bodies[j].r - dis) * STIFFNESS * dx / dis;
                ay -= (bodies[i].r + bodies[j].r - dis) * STIFFNESS * dy / dis;
            }
        }
        result[shift + 2 * i] = ax;
        result[shift + 2 * i + 1] = ay;
    }

    return result;
}


void update(vector<Body> &bodies) {
    // Implementing Runge Kutta (4th degree)
    vector<double> state(4 * bodies.size());
    u_long shift = state.size() / 2;
    for (u_long i = 0; i < bodies.size(); ++i) {
        state[2 * i] = bodies[i].x;
        state[2 * i + 1] = bodies[i].y;
        state[shift + 2 * i] = bodies[i].vx;
        state[shift + 2 * i + 1] = bodies[i].vy;
    }

    auto k1 = getDotState(state, bodies);
    auto k2 = getDotState(state + k1 * (DT / 2), bodies);
    auto k3 = getDotState(state + k2 * (DT / 2), bodies);
    auto k4 = getDotState(state + k3 * DT, bodies);

    auto change = (k1 + k2 * 2 + k3 * 2 + k4) * (DT / 6);

    for (u_long i = 0; i < bodies.size(); ++i) {
        bodies[i].x += change[2 * i];
        bodies[i].y += change[2 * i + 1];
        bodies[i].vx += change[shift + 2 * i];
        bodies[i].vy += change[shift + 2 * i + 1];
        bodies[i].update_path();
    }
}


void draw(vector<Body> &bodies, sf::RenderWindow &window) {
    for (auto &body: bodies) {
        body.drawPath(window);
    }
    for (auto &body: bodies) {
        body.draw(window);
    }
}


void run_app() {
    double gTime = 0, sTime = 0, prev = timeSinceEpochMilliseconds();
    int frameCount = 0;
    bool pause = true, pause_pressed = false;

    auto videoMode = sf::VideoMode::getDesktopMode();
    videoMode.width = (u_int) ((float) videoMode.width * SCALE);
    videoMode.height = (u_int) ((float) videoMode.height * SCALE);

    sf::RenderWindow window(videoMode, "N-Body", sf::Style::Close);
    sf::Font font = getArialFont();
    sf::Text fps_text("FPS: 0", font);
    sf::Text time_text(string_format("T: %.6lf", gTime), font);
    sf::Text pause_text("PAUSE", font);

    fps_text.setCharacterSize(30);
    fps_text.setFillColor(sf::Color::White);
    fps_text.setPosition(0, 0);
    time_text.setCharacterSize(30);
    time_text.setPosition(1, 30);
    pause_text.setCharacterSize(90);
    pause_text.setPosition(
            (float) videoMode.width / 2 - pause_text.getGlobalBounds().width / 2,
            (float) videoMode.height / 2 - pause_text.getGlobalBounds().height / 2
    );
    pause_text.setFillColor(sf::Color::Red);

    vector<Body> bodies;
    fill(bodies, videoMode.width, videoMode.height);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Down && sTime < 10) sTime += 0.1;
                    if (event.key.code == sf::Keyboard::Up && sTime > 0) sTime -= 0.1;
                    if (event.key.code == sf::Keyboard::Space && !pause_pressed) {
                        pause_pressed = true;
                        pause = !pause;
                    }
                    break;
                case sf::Event::KeyReleased:
                    if (event.key.code == sf::Keyboard::Space && pause_pressed) pause_pressed = false;
                    break;
            }
        }

        window.clear();

        draw(bodies, window);
        window.draw(fps_text);
        if (pause) window.draw(pause_text);
        window.draw(time_text);

        window.display();

        if (!pause) {
            update(bodies);
            time_text.setString(string_format("T: %.4lf", gTime));
            gTime += DT;
        }
        if (sTime > 0) usleep((int) (sTime * 1000));
        updateFps(fps_text, prev, timeSinceEpochMilliseconds(), frameCount);
    }
}
