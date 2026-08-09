#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <string>
#include <iostream>

///////////////////////
// Variabili Globali //
///////////////////////

const char* window_title = "Sphere Battle";
const unsigned window_width = 1000;
const unsigned window_height = 800;
const float max_frame_rate = 60;
const float radius_sphere_select = 40;
const float radius_sphere_selected = 70;
const unsigned vertices = 100;
const float thickness = -3.f;
const sf::Color pressEnterColor = sf::Color(180, 180, 180);



////////////
// Struct //
////////////

struct SphereData {
    std::string name;
    sf::Color color;
    std::string imagePath;
    std::string description;
    std::string attackDescription;

    SphereData() = default;

    SphereData(std::string name, sf::Color color, std::string imagePath, std::string description, std::string attackDescription) {
        this->name = name;
        this->color = color;
        this->imagePath = imagePath;
        this->description = description;
        this->attackDescription = attackDescription;
    }
};

// Istanze di SphereData
SphereData boxer ("Boxer", sf::Color(255, 176, 0), "Utilities/Images/Orange_Glove.png", "Lottatore coraggioso", "Guantoni");
SphereData cowboy ("Cowboy", sf::Color(247, 255, 0), "Utilities/Images/Yellow_Hat.png", "Cowboy giustiziere", "Revolver");
SphereData chef ("Chef", sf::Color(0, 255, 27), "Utilities/Images/Green_Pan.png", "Cuoco formidabile", "Padella");
SphereData killer ("Killer", sf::Color(255, 0, 0), "Utilities/Images/Red_Knife.png", "Assassino spietato", "Coltello");
SphereData magic ("Magic", sf::Color(67, 0, 255), "Utilities/Images/Blue_Thunder.png", "Mago del clima", "Fulmini");
SphereData hunter ("Hunter", sf::Color(252, 0, 255), "Utilities/Images/Purple_Trap.png", "Cacciatore spietato", "Trappole");

struct Sphere {
    SphereData dati;
    sf::CircleShape ball;
    sf::Texture image;
    int max_health = 1000;
    int health = 1000;

    bool sphere_loader(float radius, SphereData dati) {
        // Informazioni
        this->dati = dati;

        // Stile
        if (!image.loadFromFile(dati.imagePath)) {
            return false;
        }
        ball.setPointCount(vertices);
        ball.setRadius(radius);
        ball.setTexture(&image);
        ball.setTextureRect(sf::IntRect({0, 0}, static_cast<sf::Vector2i>(image.getSize()))); // Generato con Gemini
        ball.setFillColor(sf::Color::White);
        ball.setOutlineColor(dati.color);
        ball.setOutlineThickness(thickness);

        // Posizione
        sf::FloatRect bounds = ball.getLocalBounds();
        ball.setOrigin({bounds.position.x + bounds.size.x / 2.0f, 
                        bounds.position.y + bounds.size.y / 2.0f});
        return true;
    }
};

struct State {
    sf::RenderWindow window;
    sf::Font font;
    Sphere ball1;
    Sphere ball2;
    unsigned selected_balls = 0;
    int schermata = 0; // 0 = Title Screen, 1 = Character Sekect, 2 = Battle Simulation, 3 = Win Screen

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
        if (!font.openFromFile("Utilities/Font/PixelifySans-VariableFont_wght.ttf")) {
            std::cerr << "Errore nel caricamento del font!" << std::endl;
            window.close();
        }
    }
};



////////////////
// Ausiliarie //
////////////////

void unselect_ball1 (State& stato) {
    stato.selected_balls = 0;
}

void unselect_ball2 (State& stato) {
    stato.selected_balls = 1;
}

void update_sphere_data(sf::Text& nameText, sf::Text& descText, sf::Text& attackText, const SphereData dati, bool isP1) {
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
        nameText.setOrigin({nameBounds.position.x + nameBounds.size.x / 2.0f, 
                            nameBounds.position.y + nameBounds.size.y / 2.0f});
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

void update_arena_text (State& stato, std::vector<sf::Text>& balls_name, std::vector<sf::Text>& balls_health) {
    stato.schermata = 2;
    
    // Variabili
    unsigned font_size = 30;
    sf::Color textColor = sf::Color::Black;

    // Primo nome
    balls_name[0].setString(stato.ball1.dati.name);
    balls_name[0].setCharacterSize(font_size);
    balls_name[0].setFillColor(textColor);
    sf::FloatRect name1Bounds = balls_name[0].getLocalBounds();
    balls_name[0].setOrigin({name1Bounds.position.x + name1Bounds.size.x / 2.0f, 
                        name1Bounds.position.y + name1Bounds.size.y / 2.0f});
    balls_name[0].setPosition({122.f, 100.f});

    // Secondo nome
    balls_name[1].setString(stato.ball2.dati.name);
    balls_name[1].setCharacterSize(font_size);
    balls_name[1].setFillColor(textColor);
    sf::FloatRect name2Bounds = balls_name[1].getLocalBounds();
    balls_name[1].setOrigin({name2Bounds.position.x + name2Bounds.size.x / 2.0f, 
                        name2Bounds.position.y + name2Bounds.size.y / 2.0f});
    balls_name[1].setPosition({window_width - 122.f, 100.f});

    // Prima vita
    balls_health[0].setString(std::to_string(stato.ball1.health));
    balls_health[0].setCharacterSize(font_size);
    balls_health[0].setFillColor(textColor);
    sf::FloatRect health1Bounds = balls_health[0].getLocalBounds();
    balls_health[0].setOrigin({health1Bounds.position.x + health1Bounds.size.x / 2.0f, 
                        health1Bounds.position.y + health1Bounds.size.y / 2.0f});
    balls_health[0].setPosition({122.f, 700.f});

    // Seconda vita
    balls_health[1].setString(std::to_string(stato.ball1.health));
    balls_health[1].setCharacterSize(font_size);
    balls_health[1].setFillColor(textColor);
    sf::FloatRect health2Bounds = balls_health[1].getLocalBounds();
    balls_health[1].setOrigin({health2Bounds.position.x + health2Bounds.size.x / 2.0f, 
                        health2Bounds.position.y + health2Bounds.size.y / 2.0f});
    balls_health[1].setPosition({window_width - 122.f, 700.f});
}



//////////
// Draw //
//////////

// Schermata del titolo
void drawTitle (State& stato, const sf::Text& titleText, const sf::Text& subtitleText) {
    stato.window.draw(titleText);
    stato.window.draw(subtitleText);
}

// Character Select
void drawSelectScreen ( State& stato, std::vector<sf::RectangleShape>& sphere_selects, std::vector<Sphere>& spheres, 
                        std::vector<sf::VertexArray>& lines, std::vector<sf::Text>& character_names, std::vector<sf::Text>& ball1_description, 
                        std::vector<sf::Text>& ball2_description, sf::Text pressEnterText) {
    stato.window.draw(pressEnterText);
    for (const auto& sphere_select : sphere_selects) {
        stato.window.draw(sphere_select);
    }
    for (const auto& sphere : spheres) {
        stato.window.draw(sphere.ball);
    }
    for (const auto& line : lines) {
        stato.window.draw(line);
    }
    for (const auto& character_name : character_names) {
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
void drawBattleSimulation(  State& stato, sf::RectangleShape arena, sf::RectangleShape health_container1, sf::RectangleShape health_container2,
                            sf::RectangleShape health_bar1, sf::RectangleShape health_bar2, std::vector<sf::Text> balls_name, std::vector<sf::Text> balls_health) {
    stato.window.draw(arena);
    stato.window.draw(health_container1);
    stato.window.draw(health_container2);
    stato.window.draw(health_bar1);
    stato.window.draw(health_bar2);
    for (const auto& ball_name : balls_name) {
        stato.window.draw(ball_name);
    }
    for (const auto& ball_health : balls_health) {
        stato.window.draw(ball_health);
    }
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
    pressEnterText.setFillColor(pressEnterColor);
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
                unselect_ball1(stato);
            }
            else if (stato.selected_balls == 2) {
                unselect_ball2(stato);
            }
        }
        if (keyEvent.code == sf::Keyboard::Key::Enter) {
            if (stato.selected_balls == 2) {
                update_arena_text (stato, balls_name, balls_health);
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
            update_arena_text (stato, balls_name, balls_health);
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
// loader //
////////////

// Title Screen
sf::Text TS_title_loader (sf::Font& font) {
    // Stile
    sf::Text titleText (font, "Sphere Battle", 80);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

    // Posizione
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.0f, 
                         titleBounds.position.y + titleBounds.size.y / 2.0f});
    titleText.setPosition({window_width / 2.0f, (window_height / 2.0f) - 30.0f});
    
    return titleText;
}

sf::Text TS_subtitle_loader (sf::Font& font) {
    // Stile
    sf::Text subtitleText(font, "press any key", 30);
    subtitleText.setFillColor(sf::Color(180, 180, 180));

    // Posizione
    sf::FloatRect subBounds = subtitleText.getLocalBounds();
    subtitleText.setOrigin({subBounds.position.x + subBounds.size.x / 2.0f, 
                            subBounds.position.y + subBounds.size.y / 2.0f});
    subtitleText.setPosition({window_width / 2.0f, (window_height / 2.0f) + 40.0f});

    return subtitleText;
}

// Character Select
sf::Text CS_press_enter_loader (sf::Font& font) {
    // Stile
    sf::Text pressEnterText(font, "Press Enter to start", 20);
    pressEnterText.setFillColor(pressEnterColor);

    // Posizione
    sf::FloatRect subBounds = pressEnterText.getLocalBounds();
    pressEnterText.setOrigin({subBounds.position.x + subBounds.size.x / 2.0f, 
                            subBounds.position.y + subBounds.size.y / 2.0f});
    pressEnterText.setPosition({500.f, 470.f});

    return pressEnterText;
}

std::vector<sf::RectangleShape> CS_sphere_selects_loader() {
    // Variabili
    std::vector<sf::RectangleShape> sphere_selects;
    sphere_selects.reserve(6);
    sf::Vector2f select_size(120.f, 120.f);

    // Configurazione select
    auto setupSelect = [select_size] (sf::RectangleShape& sphere_select, sf::Vector2f pos) {
        sphere_select.setSize(select_size);
        sphere_select.setFillColor(sf::Color::Transparent);
        sphere_select.setOutlineColor(sf::Color::Black);
        sphere_select.setOutlineThickness(-thickness);
        sphere_select.setPosition(pos);
    };

    // Inizializzazione e modifica valori
    sf::RectangleShape sphere_select1, sphere_select2, sphere_select3;
    sf::RectangleShape sphere_select4, sphere_select5, sphere_select6;

    setupSelect(sphere_select1, sf::Vector2f(180.f, 40.f));
    setupSelect(sphere_select2, sf::Vector2f(440.f, 40.f));
    setupSelect(sphere_select3, sf::Vector2f(700.f, 40.f));
    setupSelect(sphere_select4, sf::Vector2f(180.f, 240.f));
    setupSelect(sphere_select5, sf::Vector2f(440.f, 240.f));
    setupSelect(sphere_select6, sf::Vector2f(700.f, 240.f));

    // Push 
    sphere_selects.push_back(sphere_select1);
    sphere_selects.push_back(sphere_select2);
    sphere_selects.push_back(sphere_select3);
    sphere_selects.push_back(sphere_select4);
    sphere_selects.push_back(sphere_select5);
    sphere_selects.push_back(sphere_select6);

    return sphere_selects;
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
    std::vector<sf::VertexArray> lines (2);
    sf::VertexArray line1(sf::PrimitiveType::Lines, 2);
    sf::VertexArray line2(sf::PrimitiveType::Lines, 2);
    sf::VertexArray line3(sf::PrimitiveType::Lines, 2);
    sf::VertexArray line4(sf::PrimitiveType::Lines, 2);
    sf::VertexArray line5(sf::PrimitiveType::Lines, 2);
    sf::Color color = sf::Color::White;

    // Prima linea
    line1[0].position = sf::Vector2f(0.f, 440.f);
    line1[0].color = color;
    line1[1].position = sf::Vector2f(window_width, 440.f);
    line1[1].color = color;

    // Seconda linea
    line2[0].position = sf::Vector2f(500.f, 500.f);
    line2[0].color = color;
    line2[1].position = sf::Vector2f(500.f, window_height);
    line2[1].color = color;

    // Terza linea
    line3[0].position = sf::Vector2f(350.f, 500.f);
    line3[0].color = color;
    line3[1].position = sf::Vector2f(650.f, 500.f);
    line3[1].color = color;

    // Quarta linea
    line4[0].position = sf::Vector2f(350.f, 440.f);
    line4[0].color = color;
    line4[1].position = sf::Vector2f(350.f, 500.f);
    line4[1].color = color;

    // Quinta linea
    line5[0].position = sf::Vector2f(650.f, 440.f);
    line5[0].color = color;
    line5[1].position = sf::Vector2f(650.f, 500.f);
    line5[1].color = color;

    // Push
    lines.push_back(line1);
    lines.push_back(line2);
    lines.push_back(line3);
    lines.push_back(line4);
    lines.push_back(line5);

    return lines;
}

std::vector<sf::Text> CS_character_names_loader (sf::Font& font) {
    // variabili
    std::vector<sf::Text> character_names;
    character_names.reserve(6);
    int font_size = 30;

    // Configurazione scritte
    auto setupName = [] (sf::Text& character_name, sf::Vector2f pos) {
        // Stile
        character_name.setFillColor(sf::Color::Black);

        // Posizione
        sf::FloatRect subBounds = character_name.getLocalBounds();
        character_name.setOrigin({subBounds.position.x + subBounds.size.x / 2.0f, 
                                subBounds.position.y + subBounds.size.y / 2.0f});
        character_name.setPosition({pos.x, pos.y});
    };

    // Inizializzazione e modifica valori
    sf::Text character_name1 (font, boxer.name + " Sphere", font_size);
    sf::Text character_name2 (font, cowboy.name + " Sphere", font_size);
    sf::Text character_name3 (font, chef.name + " Sphere", font_size);
    sf::Text character_name4 (font, killer.name + " Sphere", font_size);
    sf::Text character_name5 (font, magic.name + " Sphere", font_size);
    sf::Text character_name6 (font, hunter.name + " Sphere", font_size);

    setupName(character_name1, sf::Vector2f(240.f, 200.f));
    setupName(character_name2, sf::Vector2f(500.f, 200.f));
    setupName(character_name3, sf::Vector2f(760.f, 200.f));
    setupName(character_name4, sf::Vector2f(240.f, 400.f));
    setupName(character_name5, sf::Vector2f(500.f, 400.f));
    setupName(character_name6, sf::Vector2f(760.f, 400.f));

    // Push
    character_names.push_back(character_name1);
    character_names.push_back(character_name2);
    character_names.push_back(character_name3);
    character_names.push_back(character_name4);
    character_names.push_back(character_name5);
    character_names.push_back(character_name6);
    
    // Stile
    return character_names;
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
    arena.setOrigin({subBounds.position.x + subBounds.size.x / 2.0f, 
                            subBounds.position.y + subBounds.size.y / 2.0f});
    arena.setPosition({window_width / 2.0f, window_height / 2.0f});

    return arena;
}

sf::RectangleShape BS_health_container_loader(bool isP1) {
    sf::RectangleShape health_container;

    // Stile
    health_container.setSize(sf::Vector2f(100.f, 506.f));
    health_container.setFillColor(sf::Color::Transparent);
    health_container.setOutlineColor(sf::Color::Black);
    health_container.setOutlineThickness(-thickness);
    
    // Posizione
    sf::FloatRect subBounds = health_container.getLocalBounds();
    health_container.setOrigin({subBounds.position.x + subBounds.size.x / 2.0f, 
                            subBounds.position.y + subBounds.size.y / 2.0f});
    if (isP1) {
        health_container.setPosition({sf::Vector2f(122.f, 400.f)});
    }
    else {
        health_container.setPosition({sf::Vector2f(window_width - 122.f, 400.f)});
    }

    return health_container;
}

sf::RectangleShape BS_health_bar_loader(bool isP1) {
    sf::RectangleShape health_bar;

    // Stile
    health_bar.setSize(sf::Vector2f(100.f, 506.f));
    health_bar.setFillColor(sf::Color(180, 180, 180));
    
    // Posizione
    sf::FloatRect subBounds = health_bar.getLocalBounds();
    health_bar.setOrigin({0.f, 506.f});
    if (isP1) {
        health_bar.setPosition({sf::Vector2f(72.f, window_height - 147.f)});
    }
    else {
        health_bar.setPosition({sf::Vector2f(window_width - 172.f, window_height - 147.f)});
    }

    return health_bar;
}



//////////
// Loop //
//////////

int main () {
    State stato (window_width, window_height, window_title);
    
    // Loader del Title Screen
    sf::Text TS_titleText = TS_title_loader(stato.font);
    sf::Text TS_subtitleText = TS_subtitle_loader(stato.font);

    // Loader del Character Select
    std::vector<sf::RectangleShape> CS_sphere_selects = CS_sphere_selects_loader();
    std::vector<Sphere> CS_spheres = CS_spheres_loader(stato);
    std::vector<sf::VertexArray> CS_lines = CS_lines_loader();
    std::vector<sf::Text> CS_character_names = CS_character_names_loader(stato.font);
    sf::Text CS_pressEnterText = CS_press_enter_loader(stato.font);
    std::vector<sf::Text> CS_ball1_description(3, sf::Text(stato.font));
    std::vector<sf::Text> CS_ball2_description(3, sf::Text(stato.font));

    // Loader della Battle Simulation
    sf::RectangleShape BS_arena = BS_arena_loader();
    sf::RectangleShape BS_health_container1 = BS_health_container_loader(true);
    sf::RectangleShape BS_health_container2 = BS_health_container_loader(false);
    sf::RectangleShape BS_health_bar1 = BS_health_bar_loader(true);
    sf::RectangleShape BS_health_bar2 = BS_health_bar_loader(false);
    std::vector<sf::Text> BS_balls_name(2, sf::Text(stato.font));
    std::vector<sf::Text> BS_balls_health(2, sf::Text(stato.font));

    // Loop principale
    while (stato.window.isOpen()) {
        
        // Eventi
        stato.window.handleEvents (
            [&stato](const sf::Event::Closed& closeEvent) { 
                handle(stato, closeEvent); 
            },
            [&stato, &CS_pressEnterText, &BS_balls_name, &BS_balls_health](const sf::Event::KeyPressed& keyEvent) { 
                handle(stato, keyEvent, CS_pressEnterText, BS_balls_name, BS_balls_health); 
            },
            [&stato, &CS_ball1_description, &CS_ball2_description, &CS_pressEnterText, &BS_balls_name, &BS_balls_health](const sf::Event::MouseButtonPressed& mouseEvent) {
                handle(stato, mouseEvent, CS_ball1_description, CS_ball2_description, CS_pressEnterText, BS_balls_name, BS_balls_health);
            }
        );
        
        // Display
        if (stato.schermata == 0) {
            stato.window.clear(sf::Color::Black);
            drawTitle(stato, TS_titleText, TS_subtitleText);
        }
        else if (stato.schermata == 1) {
            stato.window.clear(sf::Color(0, 255, 255));
            drawSelectScreen(stato, CS_sphere_selects, CS_spheres, CS_lines, CS_character_names, CS_ball1_description, CS_ball2_description, CS_pressEnterText);
        }
        else if (stato.schermata == 2) {
            stato.window.clear(sf::Color(0, 255, 255));
            drawBattleSimulation(stato, BS_arena, BS_health_container1, BS_health_container2, BS_health_bar1, BS_health_bar2, BS_balls_name, BS_balls_health);
        }
        stato.window.display();
    }

    return 0;
}
