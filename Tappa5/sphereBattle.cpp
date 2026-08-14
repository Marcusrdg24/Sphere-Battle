#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>



///////////////////////
// Variabili Globali //
///////////////////////

// Informazioni window
const char* window_title = "Sphere Battle";
const unsigned window_width = 1000;
const unsigned window_height = 800;
const float max_frame_rate = 60;

// Informazioni sfere
const float radius_sphere_select = 40;
const float radius_sphere_selected = 70;
const float radius_sphere_simulation = 50;
const unsigned vertices = 100;
const int speed = 5;

// Informazioni bordi
const float thickness = -3.f;

// Informazioni arena
const float left_wall = 250.f;
const float right_wall = 750.f;
const float top_wall = 150.f;
const float bottom_wall = 650.f;

// Informazioni colori
const sf::Color pressEnterColor = sf::Color(180, 180, 180);



/////////////////
// Riflessioni //
/////////////////

sf::Angle reflect_horizontal (sf::Angle a) { // Presa dal laboratorio e modificata
    sf::Vector2f v(1.f, a);
    v.x = -v.x;
    sf::Angle reflectedAngle = v.angle();

    // Offset
    float offset = static_cast<float>((std::rand() % 21) -10);
    sf::Angle offsetAngle = reflectedAngle + sf::degrees(offset);

    // Controllo della nuova direzione
    sf::Vector2f vOffset(1.f, offsetAngle);
    if (v.x * vOffset.x <= 0.f) { // Controllo ottimizzato con Gemini
        return reflectedAngle;
    }
    return offsetAngle;
}

sf::Angle reflect_vertical (sf::Angle a) { // Presa dal laboratorio e modificata
    sf::Vector2f v(1.f, a);
    v.y = -v.y;
    sf::Angle reflectedAngle = v.angle();

    // Offset
    float offset = static_cast<float>((std::rand() % 11) -5);
    sf::Angle offsetAngle = reflectedAngle + sf::degrees(offset);

    // Controllo della nuova direzione
    sf::Vector2f vOffset(1.f, offsetAngle);
    if (v.y * vOffset.y <= 0.f) { // Controllo ottimizzato con Gemini
        return reflectedAngle;
    }
    return offsetAngle;
}



////////////
// Struct //
////////////

struct Health {
    int maxHealth = 1000;
    int health = maxHealth;
    sf::RectangleShape healthBar;
    
    Health() = default;

    Health (bool isP1) {
        // Stile
        healthBar.setSize(sf::Vector2f(100.f, 506.f));
        healthBar.setFillColor(sf::Color(180, 180, 180));
        
        // Posizione
        healthBar.setOrigin({0.f, 506.f});
        if (isP1) {
            healthBar.setPosition({sf::Vector2f(72.f, window_height - 147.f)});
        }
        else {
            healthBar.setPosition({sf::Vector2f(window_width - 172.f, window_height - 147.f)});
        }
    }

    void modifyHealth (int modifier, unsigned& schermata) {
        health -= modifier;
        if (health <= 0) {
            health = 0;
            schermata = 3;
        }
        else if (health > maxHealth) {
            health = maxHealth;
        }
        float newHeight = (static_cast<float>(health) / maxHealth) * 506.f;
        healthBar.setSize({100.f, newHeight});
        healthBar.setOrigin({0.f, newHeight});
    }
};

struct SphereData {
    std::string name;
    sf::Color color;
    std::string imagePath;
    std::string weaponPath;
    std::string description;
    std::string attackDescription;
    int cooldown;
    int strength;

    SphereData() = default;

    SphereData(std::string name, sf::Color color, std::string imagePath, std::string weaponPath, std::string description, std::string attackDescription, int cooldown, int strength) {
        this->name = name;
        this->color = color;
        this->imagePath = imagePath;
        this->weaponPath = weaponPath;
        this->description = description;
        this->attackDescription = attackDescription;
        this->cooldown = cooldown;
        this->strength = strength;
    }
};

// Istanze di SphereData
SphereData boxer ("Boxer", sf::Color(255, 176, 0), "../Utilities/Images/Orange_Glove.png", "../Utilities/Weapons/Orange_Glove.png", "Lottatore coraggioso", "Guantoni", 0, 100);
SphereData cowboy ("Cowboy", sf::Color(247, 255, 0), "../Utilities/Images/Yellow_Hat.png", "../Utilities/Weapons/Yellow_Revolver.png", "Cowboy giustiziere", "Revolver", max_frame_rate * 5, 100);
SphereData chef ("Chef", sf::Color(0, 255, 27), "../Utilities/Images/Green_Pan.png", "../Utilities/Weapons/Green_Food.png", "Cuoco formidabile", "Padella", max_frame_rate * 30, 100);
SphereData killer ("Killer", sf::Color(255, 0, 0), "../Utilities/Images/Red_Knife.png", "../Utilities/Weapons/Red_Knife.png", "Assassino spietato", "Coltello", max_frame_rate * 20, 100);
SphereData magic ("Magic", sf::Color(67, 0, 255), "../Utilities/Images/Blue_Thunder.png", "../Utilities/Weapons/Blue_Thunder.png", "Mago del clima", "Fulmini", max_frame_rate * 50, 100);
SphereData hunter ("Hunter", sf::Color(252, 0, 255), "../Utilities/Images/Purple_Trap.png", "../Utilities/Weapons/Purple_Trap.png", "Cacciatore spietato", "Trappole", max_frame_rate * 60, 100);

struct Sphere {
    SphereData dati;
    sf::CircleShape ball;
    sf::Texture image;
    sf::Texture weapon;
    sf::Sprite weaponSprite = sf::Sprite(weapon);
    Health health;
    sf::Angle angle;

    bool sphere_loader(float radius, SphereData dati) {
        // Informazioni
        this->dati = dati;

        // Stile palla
        if (!image.loadFromFile(dati.imagePath)) {
            return false;
        }
        ball.setPointCount(vertices);
        ball.setRadius(radius);
        ball.setTexture(&image);
        ball.setTextureRect(sf::IntRect({0, 0}, static_cast<sf::Vector2i>(image.getSize()))); // Generato da Gemini
        ball.setFillColor(sf::Color::White);
        ball.setOutlineColor(dati.color);
        ball.setOutlineThickness(thickness);

        // Posizione palla
        sf::FloatRect ballBounds = ball.getLocalBounds();
        ball.setOrigin({ballBounds.position.x + ballBounds.size.x / 2.f, 
                        ballBounds.position.y + ballBounds.size.y / 2.f});

        // Stile arma
        if (!weapon.loadFromFile(dati.weaponPath)) {
            return false;
        }
        weaponSprite.setTexture(weapon, true);

        // Posizione arma
        sf::FloatRect weaponBounds = weaponSprite.getLocalBounds();
        weaponSprite.setOrigin({weaponBounds.position.x + weaponBounds.size.x / 2.f, 
                        weaponBounds.position.y + weaponBounds.size.y / 2.f});

        return true;
    }

    void move () {
        sf::Vector2f displacement (speed, angle);
        ball.setPosition(ball.getPosition() + displacement);
    }

    void attack(Sphere& target, unsigned& schermata, sf::RenderWindow& window) {
        if(dati.name == "Boxer") {
            // Vettori di posizione
            sf::Vector2f pos = ball.getPosition();
            sf::Vector2f targetPos = target.ball.getPosition();
            sf::Vector2f targetDirection = targetPos - pos;

            // Posizione guanto
            sf::Angle targetAngle = targetDirection.angle();
            sf::Vector2f glovePos = pos + sf::Vector2f(radius_sphere_simulation + (weapon.getSize().y / 2.f), targetAngle);
            weaponSprite.setPosition(glovePos);
            weaponSprite.setRotation(targetAngle + sf::degrees(90));

        
            // Controllo collisione sfere (Generato da Gemini e adattato per la collisione con aggiunta di guanto)
            // Distanze
            sf::Vector2f delta = targetPos - pos;
            float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
            float minDistance = radius_sphere_simulation * 2.f + weapon.getSize().y;
            if (target.dati.name == "Boxer") {
                minDistance += weapon.getSize().y;
            }

            // Scontro
            if (distance < minDistance && distance > 0.f) {
                // Vettore normalizzato
                sf::Vector2f normal = delta / distance;

                // Sovrapposizione
                float overlap = 0.5f * (minDistance - distance);
                ball.setPosition(pos - normal * overlap);
                target.ball.setPosition(targetPos + normal * overlap);

                // Vettori velocità
                sf::Vector2f v1(1.f, angle);
                sf::Vector2f v2(1.f, target.angle);

                // Vettore relativo
                sf::Vector2f vRel = v1 - v2;
                float dotProduct = vRel.x * normal.x + vRel.y * normal.y;

                // Impulso
                if (dotProduct > 0.f) {
                    //Nuovi angoli
                    v1 -= dotProduct * normal;
                    v2 += dotProduct * normal;
                    angle = v1.angle();
                    target.angle = v2.angle();

                    // Danno
                    target.health.modifyHealth(dati.strength, schermata);
                    if (target.dati.name == "Boxer") {
                        health.modifyHealth(target.dati.strength, schermata);
                    }
                }
            }
            window.draw(weaponSprite);
        }
    }
};

struct State {
    sf::RenderWindow window;
    sf::Font font;
    Sphere ball1;
    Sphere ball2;
    unsigned selected_balls = 0;
    unsigned schermata = 0; // 0 = Title Screen, 1 = Character Select, 2 = Battle Simulation, 3 = Win Screen

    State() = default;

    State(unsigned w, unsigned h, std::string title) {
        sf::ContextSettings settings;
        settings.antiAliasingLevel = 4;

        // Creazione Window
        window = sf::RenderWindow(sf::VideoMode({w, h}), title, sf::Style::Default, sf::State::Windowed, settings);
        window.setFramerateLimit(max_frame_rate);
        window.setMinimumSize(window.getSize());

        // Posizione Window (Generato da gemini)
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        int posX = static_cast<int>((desktop.size.x - w) / 2);
        int posY = static_cast<int>((desktop.size.y - h) / 2);
        window.setPosition(sf::Vector2i(posX, posY));

        // Font
        if (!font.openFromFile("../Utilities/Font/PixelifySans-VariableFont_wght.ttf")) {
            std::cerr << "Errore nel caricamento del font!" << std::endl;
            window.close();
        }
    }

    void collisions () {
        // Controllo collisione muri
        auto checkWallCollisions = [this] (Sphere& sphere) {
            // Variabili
            sf::Vector2f pos = sphere.ball.getPosition();
            sf::Vector2f dir(1.f, sphere.angle);

            // Muro sinistro
            if (pos.x - radius_sphere_simulation <= left_wall && dir.x < 0) {
                sphere.angle = reflect_horizontal(sphere.angle);
                sphere.ball.setPosition({left_wall + radius_sphere_simulation, pos.y});
            }

            // Muro destro
            else if (pos.x + radius_sphere_simulation >= right_wall && dir.x > 0) {
                sphere.angle = reflect_horizontal(sphere.angle);
                sphere.ball.setPosition({right_wall - radius_sphere_simulation, pos.y});
            }

            pos = sphere.ball.getPosition();
            dir = sf::Vector2f(1.f, sphere.angle);

            // Muro superiore
            if (pos.y - radius_sphere_simulation <= top_wall && dir.y < 0) {
                sphere.angle = reflect_vertical(sphere.angle);
                sphere.ball.setPosition({pos.x, top_wall + radius_sphere_simulation});
            }

            // Muro inferiore
            else if (pos.y + radius_sphere_simulation >= bottom_wall && dir.y > 0) {
                sphere.angle = reflect_vertical(sphere.angle);
                sphere.ball.setPosition({pos.x, bottom_wall - radius_sphere_simulation});
            }
        };

        checkWallCollisions(ball1);
        checkWallCollisions(ball2);

        // Controllo collisione sfere (Generato da Gemini)
        sf::Vector2f pos1 = ball1.ball.getPosition();
        sf::Vector2f pos2 = ball2.ball.getPosition();

        // Distanze
        sf::Vector2f delta = pos2 - pos1;
        float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        float minDistance = radius_sphere_simulation * 2.f;

        // Scontro
        if (distance < minDistance && distance > 0.f) {
            // Vettore normalizzato
            sf::Vector2f normal = delta / distance;

            // Sovrapposizione
            float overlap = 0.5f * (minDistance - distance);
            ball1.ball.setPosition(pos1 - normal * overlap);
            ball2.ball.setPosition(pos2 + normal * overlap);

            // Vettori velocità
            sf::Vector2f v1(1.f, ball1.angle);
            sf::Vector2f v2(1.f, ball2.angle);

            // Vettore relativo
            sf::Vector2f vRel = v1 - v2;
            float dotProduct = vRel.x * normal.x + vRel.y * normal.y;

            // Impulso
            if (dotProduct > 0.f) {
                //Nuovi angoli
                v1 -= dotProduct * normal;
                v2 += dotProduct * normal;
                ball1.angle = v1.angle();
                ball2.angle = v2.angle();
            }
        }
    }
};



////////////////
// Ausiliarie //
////////////////

void update_sphere_data(sf::Text& nameText, sf::Text& descText, sf::Text& attackText, const SphereData& dati, bool isP1) {
    unsigned font_size_title = 40;
    unsigned font_size_body = 23;
    sf::Color textColor = sf::Color::Black;

    // Nome
    nameText.setString(dati.name);
    nameText.setCharacterSize(font_size_title);
    nameText.setFillColor(textColor);
    nameText.setStyle(sf::Text::Bold);

    // Posizione
    sf::FloatRect nameBounds = nameText.getLocalBounds();
        nameText.setOrigin({nameBounds.position.x + nameBounds.size.x / 2.f, 
                            nameBounds.position.y + nameBounds.size.y / 2.f});
    if (isP1) {
        nameText.setPosition({110.f, 530.f});
    }
    else {
        nameText.setPosition({890.f, 530.f});
    }

    // Descrizione
    descText.setString(dati.description);
    descText.setCharacterSize(font_size_body);
    descText.setFillColor(textColor);
    if (isP1) {
        descText.setPosition({210.f, 600.f});
    }
    else {
        descText.setPosition({530.f, 600.f});
    }

    // Descrizione attacco
    attackText.setString("Attacco: " + dati.attackDescription);
    attackText.setCharacterSize(font_size_body);
    attackText.setFillColor(textColor);
    if (isP1) {
        attackText.setPosition({210.f, 640.f});
    }
    else {
        attackText.setPosition({530.f, 640.f});
    }
}

void initialize_arena (State& stato, std::vector<sf::Text>& balls_name, std::vector<sf::Text>& balls_health) {
    stato.schermata = 2;
    
    // Variabili
    unsigned font_size = 30;
    sf::Color textColor = sf::Color::Black;

    // Primo nome
    balls_name[0].setString(stato.ball1.dati.name);
    balls_name[0].setCharacterSize(font_size);
    balls_name[0].setFillColor(textColor);
    sf::FloatRect name1Bounds = balls_name[0].getLocalBounds();
    balls_name[0].setOrigin({name1Bounds.position.x + name1Bounds.size.x / 2.f, 
                        name1Bounds.position.y + name1Bounds.size.y / 2.f});
    balls_name[0].setPosition({122.f, 100.f});

    // Secondo nome
    balls_name[1].setString(stato.ball2.dati.name);
    balls_name[1].setCharacterSize(font_size);
    balls_name[1].setFillColor(textColor);
    sf::FloatRect name2Bounds = balls_name[1].getLocalBounds();
    balls_name[1].setOrigin({name2Bounds.position.x + name2Bounds.size.x / 2.f, 
                        name2Bounds.position.y + name2Bounds.size.y / 2.f});
    balls_name[1].setPosition({window_width - 122.f, 100.f});

    // Prima vita
    balls_health[0].setString(std::to_string(stato.ball1.health.health));
    balls_health[0].setCharacterSize(font_size);
    balls_health[0].setFillColor(textColor);
    sf::FloatRect health1Bounds = balls_health[0].getLocalBounds();
    balls_health[0].setOrigin({health1Bounds.position.x + health1Bounds.size.x / 2.f, 
                        health1Bounds.position.y + health1Bounds.size.y / 2.f});
    balls_health[0].setPosition({122.f, 700.f});

    // Seconda vita
    balls_health[1].setString(std::to_string(stato.ball2.health.health));
    balls_health[1].setCharacterSize(font_size);
    balls_health[1].setFillColor(textColor);
    sf::FloatRect health2Bounds = balls_health[1].getLocalBounds();
    balls_health[1].setOrigin({health2Bounds.position.x + health2Bounds.size.x / 2.f, 
                        health2Bounds.position.y + health2Bounds.size.y / 2.f});
    balls_health[1].setPosition({window_width - 122.f, 700.f});

    // Prima sfera
    stato.ball1.ball.setRadius(radius_sphere_simulation);
    sf::FloatRect ball1Bounds = stato.ball1.ball.getLocalBounds();
    stato.ball1.ball.setOrigin({ball1Bounds.position.x + ball1Bounds.size.x / 2.f, 
                        ball1Bounds.position.y + ball1Bounds.size.y / 2.f});
    stato.ball1.ball.setPosition({left_wall + radius_sphere_simulation + 40.f, 400.f});
    stato.ball1.angle = sf::degrees(rand() % 360);
    stato.ball1.health = Health(true);

    // Seconda sfera
    stato.ball2.ball.setRadius(radius_sphere_simulation);
    sf::FloatRect ball2Bounds = stato.ball2.ball.getLocalBounds();
    stato.ball2.ball.setOrigin({ball2Bounds.position.x + ball2Bounds.size.x / 2.f, 
                        ball2Bounds.position.y + ball2Bounds.size.y / 2.f});
    stato.ball2.ball.setPosition({right_wall - radius_sphere_simulation - 40.f, 400.f});
    stato.ball2.angle = sf::degrees(rand() % 360);
    stato.ball2.health = Health(false);
}

void initialize_winScreen (State& stato, sf::Text& winnerText) {
    // Scritta
    if (stato.ball1.health.health > 0 && stato.ball2.health.health > 0) {
        std::cerr << "Errore nel cambio di schermata" << std::endl;
        stato.window.close();
    }
    else if (stato.ball1.health.health == 0 && stato.ball2.health.health == 0) {
        winnerText.setString("Pareggio");
    }
    else if (stato.ball1.health.health == 0) {
        winnerText.setString(winnerText.getString() + stato.ball2.dati.name);
    }
    else if (stato.ball2.health.health == 0) {
        winnerText.setString(winnerText.getString() + stato.ball1.dati.name);
    }

    // Stile
    winnerText.setFillColor(sf::Color::Black);

    // Posizione
    sf::FloatRect titleBounds = winnerText.getLocalBounds();
    winnerText.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.f, 
                         titleBounds.position.y + titleBounds.size.y / 2.f});
    winnerText.setPosition({window_width / 2.f, window_height / 2.f});
}

void moveGame (State& stato) {
    stato.collisions();
    stato.ball1.move();
    stato.ball2.move();
    stato.ball1.attack(stato.ball2, stato.schermata, stato.window);
    stato.ball2.attack(stato.ball1, stato.schermata, stato.window);
}



//////////
// Draw //
//////////

// Title Screen
void drawTitleScreen (State& stato, const sf::Text& titleText, const sf::Text& subtitleText) {
    stato.window.draw(titleText);
    stato.window.draw(subtitleText);
}

// Character Select
void drawSelectScreen ( State& stato, std::vector<sf::RectangleShape>& sphereSelects, std::vector<Sphere>& spheres, 
                        std::vector<sf::VertexArray>& lines, std::vector<sf::Text>& characterNames, std::vector<sf::Text>& ball1_description, 
                        std::vector<sf::Text>& ball2_description, sf::Text pressEnterText) {
    stato.window.draw(pressEnterText);
    for (const auto& sphere_select : sphereSelects) {
        stato.window.draw(sphere_select);
    }
    for (const auto& sphere : spheres) {
        stato.window.draw(sphere.ball);
    }
    for (const auto& line : lines) {
        stato.window.draw(line);
    }
    for (const auto& character_name : characterNames) {
        stato.window.draw(character_name);
    }
    if (stato.selected_balls > 0) {
        stato.window.draw(stato.ball1.ball);
        for (const auto& description : ball1_description) {
            stato.window.draw(description);
        }
    }
    if (stato.selected_balls > 1) {
        stato.window.draw(stato.ball2.ball);
        for (const auto& description : ball2_description) {
            stato.window.draw(description);
        }
    }
}

// Battle Simulation
void drawBattleSimulation(  State& stato, sf::RectangleShape arena, sf::RectangleShape healthContainer1, sf::RectangleShape healthContainer2,
                            std::vector<sf::Text> balls_name, std::vector<sf::Text> balls_health) {
    stato.window.draw(arena);
    stato.window.draw(healthContainer1);
    stato.window.draw(healthContainer2);
    stato.window.draw(stato.ball1.health.healthBar);
    stato.window.draw(stato.ball2.health.healthBar);
    for (const auto& ball_name : balls_name) {
        stato.window.draw(ball_name);
    }
    balls_health[0].setString(std::to_string(stato.ball1.health.health));
    balls_health[1].setString(std::to_string(stato.ball2.health.health));
    for (const auto& ball_health : balls_health) {
        stato.window.draw(ball_health);
    }
    moveGame(stato);
    stato.window.draw(stato.ball1.ball);
    stato.window.draw(stato.ball2.ball);
}

// Win Screen
void drawWinScreen (State& stato, sf::Text winnerText) {
    stato.window.draw(winnerText);
}



////////////
// Handle //
////////////

// Chiusura finestra
void handle (State& stato, const sf::Event::Closed& closeEvent) {
    stato.window.close();
}

// Pressione tasto
void handle (State& stato, const sf::Event::KeyPressed& keyEvent, sf::Text& pressEnterText, std::vector<sf::Text>& balls_name, std::vector<sf::Text>& balls_health) {
    if (stato.schermata == 0) {
        if (keyEvent.code != sf::Keyboard::Key::Escape) {
            stato.schermata = 1;
        }
    }
    else if (stato.schermata == 1) {
        if (keyEvent.code == sf::Keyboard::Key::Escape) {
            if (stato.selected_balls == 0) {
                stato.schermata = 0;
            }
            else if (stato.selected_balls == 1) {
                stato.selected_balls = 0;
            }
            else if (stato.selected_balls == 2) {
                pressEnterText.setFillColor(pressEnterColor);
                stato.selected_balls = 1;
            }
        }
        else if (keyEvent.code == sf::Keyboard::Key::Enter) {
            if (stato.selected_balls == 2) {
                initialize_arena (stato, balls_name, balls_health);
            }
        }
    }
}

// Click mouse
void handle (   State& stato, const sf::Event::MouseButtonPressed& mouseEvent, std::vector<sf::Text>& ball1_description, 
                std::vector<sf::Text>& ball2_description, sf::Text& pressEnterText, std::vector<sf::Text>& balls_name, std::vector<sf::Text>& balls_health) {
    if (stato.schermata == 0) {
        stato.schermata = 1;
    }
    else if (stato.schermata == 1) {
        // Variabili
        sf::Vector2f pos = stato.window.mapPixelToCoords(mouseEvent.position);
        SphereData selectedSphere;

        // Selezione personaggio
        if (pos.y > 40 && pos.y < 160) {
            if (pos.x > 180 && pos.x < 300) {
                selectedSphere = boxer;
            }
            else if (pos.x > 440 && pos.x < 560) {
                selectedSphere = cowboy;
            }
            else if (pos.x > 700 && pos.x < 820) {
                selectedSphere = chef;
            }
        }
        else if (pos.y > 240 && pos.y < 360) {
            if (pos.x > 180 && pos.x < 300) {
                selectedSphere = killer;
            }
            else if (pos.x > 440 && pos.x < 560) {
                selectedSphere = magic;
            }
            else if (pos.x > 700 && pos.x < 820) {
                selectedSphere = hunter;
            }
        }

        // Avvio simulazione
        else if (pos.y > 440.f && pos.y < 500.f && pos.x > 350 && pos.x < 650) {
            initialize_arena (stato, balls_name, balls_health);
        }

        // Aggiornamento informazioni personaggio
        if (!selectedSphere.imagePath.empty()) {
            if (stato.selected_balls == 0) {
                if (!stato.ball1.sphere_loader(radius_sphere_selected, selectedSphere)) {
                    stato.window.close();
                }
                stato.ball1.ball.setPosition(sf::Vector2f(110.f, 640.f));
                update_sphere_data(ball1_description[0], ball1_description[1], ball1_description[2], selectedSphere, true);
                stato.selected_balls = 1;
            }
            else if (stato.selected_balls == 1) {
                if (!stato.ball2.sphere_loader(radius_sphere_selected, selectedSphere)) {
                    stato.window.close();
                }
                stato.ball2.ball.setPosition(sf::Vector2f(890.f, 640.f));
                update_sphere_data(ball2_description[0], ball2_description[1], ball2_description[2], selectedSphere, false);
                pressEnterText.setFillColor(sf::Color::Black);
                stato.selected_balls = 2;
            }
        }
    }
}



////////////
// Loader //
////////////

// Title Screen
sf::Text TS_titleText_loader (sf::Font& font) {
    // Stile
    sf::Text titleText (font, "Sphere Battle", 80);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

    // Posizione
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.f, 
                         titleBounds.position.y + titleBounds.size.y / 2.f});
    titleText.setPosition({window_width / 2.f, (window_height / 2.f) - 30.f});
    
    return titleText;
}

sf::Text TS_subtitleText_loader (sf::Font& font) {
    // Stile
    sf::Text subtitleText(font, "press any key", 30);
    subtitleText.setFillColor(sf::Color(180, 180, 180));

    // Posizione
    sf::FloatRect subBounds = subtitleText.getLocalBounds();
    subtitleText.setOrigin({subBounds.position.x + subBounds.size.x / 2.f, 
                            subBounds.position.y + subBounds.size.y / 2.f});
    subtitleText.setPosition({window_width / 2.f, (window_height / 2.f) + 40.f});

    return subtitleText;
}

// Character Select
sf::Text CS_pressEnterText_loader (sf::Font& font) {
    // Stile
    sf::Text pressEnterText(font, "Press Enter to start", 20);
    pressEnterText.setFillColor(pressEnterColor);

    // Posizione
    sf::FloatRect subBounds = pressEnterText.getLocalBounds();
    pressEnterText.setOrigin({subBounds.position.x + subBounds.size.x / 2.f, 
                            subBounds.position.y + subBounds.size.y / 2.f});
    pressEnterText.setPosition({500.f, 470.f});

    return pressEnterText;
}

std::vector<sf::RectangleShape> CS_sphereSelects_loader() {
    // Variabili
    std::vector<sf::RectangleShape> sphereSelects;
    sphereSelects.reserve(6);
    const sf::Vector2f select_size(120.f, 120.f);

    // Posizioni
    sf::Vector2f pos[6] = {
        {180.f, 40.f},  {440.f, 40.f},  {700.f, 40.f},
        {180.f, 240.f}, {440.f, 240.f}, {700.f, 240.f}
    };

    // Creazione rettangoli e inserimento nel vettore
    for (int i = 0; i < 6; i++) {
        sf::RectangleShape sphere_select(select_size);
        sphere_select.setFillColor(sf::Color::Transparent);
        sphere_select.setOutlineColor(sf::Color::Black);
        sphere_select.setOutlineThickness(-thickness);
        sphere_select.setPosition(pos[i]);
        sphereSelects.push_back(sphere_select);
    }

    return sphereSelects;
}

std::vector<Sphere> CS_spheres_loader(State& stato) { // Può ritornare errore in caso di file mancante
    // Variabili
    std::vector<Sphere> spheres(6);

    // Configurazione sfere
    auto setupSphere = [&stato] (Sphere& sphere, SphereData dati, sf::Vector2f pos) {
        if (sphere.sphere_loader(radius_sphere_select, dati)) {
            sphere.ball.setPosition(pos);
        }
        else {
            stato.window.close();
        }
    };

    setupSphere(spheres[0], boxer, sf::Vector2f(240.f, 100.f));
    setupSphere(spheres[1], cowboy, sf::Vector2f(500.f, 100.f));
    setupSphere(spheres[2], chef, sf::Vector2f(760.f, 100.f));
    setupSphere(spheres[3], killer, sf::Vector2f(240.f, 300.f));
    setupSphere(spheres[4], magic, sf::Vector2f(500.f, 300.f));
    setupSphere(spheres[5], hunter, sf::Vector2f(760.f, 300.f));

    return spheres;
}

std::vector<sf::VertexArray> CS_lines_loader () {
    // Variabili
    std::vector<sf::VertexArray> lines;
    lines.reserve(5);
    const sf::Color color = sf::Color::White;

    // Configurazione linee
    auto setupLine = [color] (sf::Vector2f pos1, sf::Vector2f pos2) {
        sf::VertexArray line(sf::PrimitiveType::Lines, 2);
        line[0].position = pos1;
        line[0].color = color;
        line[1].position = pos2;
        line[1].color = color;
        return line;
    };
    
    // Creazione linee e inserimento nel vettore
    lines.push_back(setupLine({0.f, 440.f}, {window_width, 440.f}));
    lines.push_back(setupLine({500.f, 500.f}, {500.f, window_height}));
    lines.push_back(setupLine({350.f, 500.f}, {650.f, 500.f}));
    lines.push_back(setupLine({350.f, 440.f}, {350.f, 500.f}));
    lines.push_back(setupLine({650.f, 440.f}, {650.f, 500.f}));

    return lines;
}

std::vector<sf::Text> CS_characterNames_loader (sf::Font& font) {
    // variabili
    std::vector<sf::Text> characterNames;
    characterNames.reserve(6);
    const unsigned font_size = 30;

    // Nomi
    SphereData data[6] = {
        boxer, cowboy, chef, killer, magic, hunter
    };

    // Posizioni
    sf::Vector2f pos[6] = {
        {240.f, 200.f}, {500.f, 200.f}, {760.f, 200.f},
        {240.f, 400.f}, {500.f, 400.f}, {760.f, 400.f}
    };

    // Creazione nomi e inserimento nel vettore
    for (int i = 0; i < 6; i++) {
        sf::Text character_name(font, data[i].name + " Sphere", font_size);

        // Stile
        character_name.setFillColor(sf::Color::Black);

        // Posizione
        sf::FloatRect bounds = character_name.getLocalBounds();
        character_name.setOrigin({bounds.position.x + bounds.size.x / 2.f, 
                            bounds.position.y + bounds.size.y / 2.f});
        character_name.setPosition(pos[i]);

        characterNames.push_back(character_name);
    }
    
    return characterNames;
}

// Battle Simulation
sf::RectangleShape BS_arena_loader() {
    sf::RectangleShape arena;

    // Stile
    arena.setSize(sf::Vector2f(500.f, 500.f));
    arena.setFillColor(sf::Color::Transparent);
    arena.setOutlineColor(sf::Color::Black);
    arena.setOutlineThickness(-thickness * 2);
    
    // Posizione
    sf::FloatRect subBounds = arena.getLocalBounds();
    arena.setOrigin({subBounds.position.x + subBounds.size.x / 2.f, 
                            subBounds.position.y + subBounds.size.y / 2.f});
    arena.setPosition({window_width / 2.f, window_height / 2.f});

    return arena;
}

sf::RectangleShape BS_healthContainer_loader(bool isP1) {
    sf::RectangleShape healthContainer;

    // Stile
    healthContainer.setSize(sf::Vector2f(100.f, 506.f));
    healthContainer.setFillColor(sf::Color::Transparent);
    healthContainer.setOutlineColor(sf::Color::Black);
    healthContainer.setOutlineThickness(-thickness);
    
    // Posizione
    sf::FloatRect subBounds = healthContainer.getLocalBounds();
    healthContainer.setOrigin({subBounds.position.x + subBounds.size.x / 2.f, 
                            subBounds.position.y + subBounds.size.y / 2.f});
    if (isP1) {
        healthContainer.setPosition({sf::Vector2f(122.f, 400.f)});
    }
    else {
        healthContainer.setPosition({sf::Vector2f(window_width - 122.f, 400.f)});
    }

    return healthContainer;
}



//////////
// Loop //
//////////

int main () {
    State stato (window_width, window_height, window_title);
    
    // Loader del Title Screen
    sf::Text TS_titleText = TS_titleText_loader(stato.font);
    sf::Text TS_subtitleText = TS_subtitleText_loader(stato.font);

    // Loader del Character Select
    std::vector<sf::RectangleShape> CS_sphereSelects = CS_sphereSelects_loader();
    std::vector<Sphere> CS_spheres = CS_spheres_loader(stato);
    std::vector<sf::VertexArray> CS_lines = CS_lines_loader();
    std::vector<sf::Text> CS_characterNames = CS_characterNames_loader(stato.font);
    sf::Text CS_pressEnterText = CS_pressEnterText_loader(stato.font);
    std::vector<sf::Text> CS_ballDescription1(3, sf::Text(stato.font));
    std::vector<sf::Text> CS_ballDescription2(3, sf::Text(stato.font));

    // Loader della Battle Simulation
    sf::RectangleShape BS_arena = BS_arena_loader();
    sf::RectangleShape BS_healthContainer1 = BS_healthContainer_loader(true);
    sf::RectangleShape BS_healthContainer2 = BS_healthContainer_loader(false);
    std::vector<sf::Text> BS_ballsName(2, sf::Text(stato.font));
    std::vector<sf::Text> BS_ballsHealth(2, sf::Text(stato.font));

    // Loader del Win Screen
    sf::Text WS_winnerText (stato.font, "Vincitore: ", 60);

    // Loop principale
    while (stato.window.isOpen()) {
        
        // Eventi
        stato.window.handleEvents (
            [&stato](const sf::Event::Closed& closeEvent) { 
                handle(stato, closeEvent); 
            },
            [&stato, &CS_pressEnterText, &BS_ballsName, &BS_ballsHealth](const sf::Event::KeyPressed& keyEvent) { 
                handle(stato, keyEvent, CS_pressEnterText, BS_ballsName, BS_ballsHealth); 
            },
            [&stato, &CS_ballDescription1, &CS_ballDescription2, &CS_pressEnterText, &BS_ballsName, &BS_ballsHealth](const sf::Event::MouseButtonPressed& mouseEvent) {
                handle(stato, mouseEvent, CS_ballDescription1, CS_ballDescription2, CS_pressEnterText, BS_ballsName, BS_ballsHealth);
            }
        );
        
        // Display
        if (stato.schermata == 0) {
            stato.window.clear(sf::Color::Black);
            drawTitleScreen(stato, TS_titleText, TS_subtitleText);
        }
        else if (stato.schermata == 1) {
            stato.window.clear(sf::Color(0, 255, 255));
            drawSelectScreen(stato, CS_sphereSelects, CS_spheres, CS_lines, CS_characterNames, CS_ballDescription1, CS_ballDescription2, CS_pressEnterText);
        }
        else if (stato.schermata == 2) {
            stato.window.clear(sf::Color(0, 255, 255));
            drawBattleSimulation(stato, BS_arena, BS_healthContainer1, BS_healthContainer2, BS_ballsName, BS_ballsHealth);
            // Cambio schermata in questo frame
            if (stato.schermata == 3) {
                initialize_winScreen(stato, WS_winnerText);
            }
        }
        else if (stato.schermata == 3) {
            stato.window.clear(sf::Color(0, 255, 255));
            drawWinScreen(stato, WS_winnerText);
        }
        stato.window.display();
    }
}
