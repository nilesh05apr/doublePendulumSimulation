#include "DPendulum.h"
#include "Constants.h"
#include <utility>
#include <numeric>
#include <vector>
#include <random>
#include <time.h>

sf::Color rainbow(int s) {
    std::vector<sf::Color> c(7);
    c[0] = sf::Color(148, 0, 211, (s + 1) * 2550 / trailsize);
    c[1] = sf::Color(75, 0, 130, (s + 1) * 2550 / trailsize);
    c[2] = sf::Color(0, 0, 255, (s + 1) * 2550 / trailsize);
    c[3] = sf::Color(0, 255, 0, (s + 1) * 2550 / trailsize);
    c[4] = sf::Color(255, 255, 0, (s + 1) * 2550 / trailsize);
    c[5] = sf::Color(255, 127, 0, (s + 1) * 2550 / trailsize);
    c[6] = sf::Color(255, 0, 0, (s + 1) * 2550 / trailsize);

    return c[rand() % 7];
}

void DPendulum::updatexy()
{
    posx.first = length.first * std::sin(angle.first);
    posy.first = length.first * std::cos(angle.first);

    posx.second = posx.first + length.second * std::sin(angle.second);
    posy.second = posy.first + length.second * std::cos(angle.second);
}

int DPendulum::bobclicked(sf::Vector2i mousepos)
{
    if (bob.first.getGlobalBounds().contains(mousepos.x, mousepos.y)) return 1;
    else if (bob.second.getGlobalBounds().contains(mousepos.x, mousepos.y)) return 2;
    else return 0;
}

DPendulum::DPendulum(float len_1, float len_2, float mass_1, float mass_2, float angle_1, float angle_2, float damp_1, float damp_2)
{
    this->length.first = len_1;
    this->length.second = len_2;

    this->mass.first = mass_1;
    this->mass.second = mass_2;

    this->angle.first = angle_1;
    this->angle.second = angle_2;

    this->damp.first = damp_1;
    this->damp.second = damp_2;

    angacl.first = 0;
    angacl.second = 0;
    angvel.first = 0;
    angvel.second = 0;

}

void DPendulum::setupRenderObjects(float width, float height)
{
    textr.create(width, height);
    this->setTexture(textr.getTexture());
    
    vb.create(3);
    vb.setPrimitiveType(sf::LinesStrip);

    rvertices[0].position = sf::Vector2f(width / 2, height / 2);
    rvertices[0].color = sf::Color::Black;
    rvertices[1].color = sf::Color::Black;
    rvertices[2].color = sf::Color::Black;

    bob.first.setRadius(mass.first);
    bob.first.setOrigin(bob.first.getRadius(), bob.first.getRadius());
    bob.first.setFillColor(sf::Color::Black);

    bob.second.setRadius(mass.second);
    bob.second.setOrigin(bob.second.getRadius(), bob.second.getRadius());
    bob.second.setFillColor(sf::Color::Black);

}

void DPendulum::update()
{
    if (hold) return;

    std::vector<std::vector<float>> n(2, std::vector<float>(4));
    n[0][0] = -1.0 * g * (2 * mass.first + mass.second) * std::sin(angle.first);
    n[0][1] = -1.0 * mass.second * g * std::sin(angle.first - 2 * angle.second);
    n[0][2] = -2.0 * std::sin(angle.first - angle.second) * mass.second;
    n[0][3] = angvel.first * angvel.first * length.first * std::cos(angle.first - angle.second) + angvel.second * angvel.second * length.second;

    float d = 2 * mass.first + mass.second * (1 - std::cos(2 * (angle.first - angle.second)));

    n[1][0] = 2 * std::sin(angle.first - angle.second);
    n[1][1] = angvel.first * angvel.first * length.first * (mass.first + mass.second);
    n[1][2] = g * (mass.first + mass.second) * std::cos(angle.first);
    n[1][3] = angvel.second * angvel.second * length.second * mass.second * std::cos(angle.first - angle.second);

    float nv_1 = (n[0][0] + n[0][1] + n[0][2] * n[0][3]);
    float nv_2 = (n[1][1] + n[1][2] + n[1][3]) * n[1][0];

    angacl.first = nv_1 / (length.first * d * FPS * FPS);
    angacl.second = nv_2 / (length.second * d * FPS * FPS);

    angvel.first += angacl.first;
    angvel.second += angacl.second;

    angle.first += angvel.first;
    angle.second += angvel.second;

    updatexy();

    angvel.first *= damp.first;
    angvel.second *= damp.second;
}

void DPendulum::render()
{
    std::pair<sf::Vector2f, sf::Vector2f> endpos;
    endpos.first = sf::Vector2f((posx.first * 100) + rvertices[0].position.x, (posy.first * 100) + rvertices[0].position.y);
    endpos.second = sf::Vector2f((posx.second * 100) + rvertices[0].position.x, (posy.second * 100) + rvertices[0].position.y);

    rvertices[1].position = endpos.first;
    rvertices[2].position = endpos.second;
    vb.update(rvertices);

    bob.first.setPosition(endpos.first);
    bob.second.setPosition(endpos.second);

    int s = trails.size();
    if (s < trailsize) {
        sf::Color color = rainbow(s);
        trails.push_back(sf::Vertex(endpos.second, color));
    }
    else {
        for (int i = 0; i < s - 1; i++) {
            trails[i].position = trails[i + 1].position;
        }
        trails[s - 1].position = endpos.second;
    }


    textr.clear(sf::Color::White);
    if (showrod) textr.draw(vb);
    textr.draw(bob.first);
    textr.draw(bob.second);
    textr.draw(&trails[0], trails.size(), sf::LinesStrip);
    textr.display();

    std::system("cls");
    std::cout << "Angulur Accelerations: " << std::endl;
    std::cout << "\tsangacl_1: " << angacl.first << "\tangacl_2: " << angacl.second << std::endl;
    std::cout << "Angulur Velocitiess: " << std::endl;
    std::cout << "\tsangvel_1: " << angvel.first << "\tangvel_2: " << angvel.second << std::endl;
    std::cout << "Angles: " << std::endl;
    std::cout << "\tsangle_1: " << angle.first << "\tangle_2: " << angle.second << std::endl;
}

void DPendulum::changestate()
{
    hold = !hold;
}

void DPendulum::changemoving(int id)
{
    switch (id)
    {
    case 1: moving.first = !moving.first; break;
    case 2: moving.second = !moving.second; break;
    }
}

void DPendulum::movebob(sf::Vector2i mousepos)
{
    if (!moving.first && !moving.second) return;

    float x = mousepos.x;
    float y = mousepos.y;

    if (moving.first) {
        float del_x = x - rvertices[0].position.x;
        float del_y = y - rvertices[0].position.y;

        angle.first = std::atan(del_x / del_y);

        if (del_y < 0) angle.first += PI;

        length.first = std::sqrt((del_x * del_x) + (del_y * del_y)) / 100;
    }
    if (moving.second) {
        float del_x = x - rvertices[1].position.x;
        float del_y = y - rvertices[1].position.y;

        angle.second = std::atan(del_x / del_y);

        if (del_y < 0) angle.second += PI;

        length.second = std::sqrt((del_x * del_x) + (del_y * del_y)) / 100;
    }

    angvel.first = 0;
    angvel.second = 0;
    updatexy();
}

void DPendulum::clicked(sf::Vector2i mousepos)
{
    int id = bobclicked(mousepos);

    if (id == 0) return;
    hold = true;

    changemoving(id);
}

void DPendulum::rodvisibility()
{
    showrod = !showrod;
}
