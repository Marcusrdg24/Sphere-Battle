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

    bool sphere_loader(float radius, SphereData dati, sf::Font font) {
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
    int schermata = 0;

    State() = default;

    State(unsigned w, unsigned h, std::string title) {
        sf::ContextSettings settings;
        settings.antiAliasingLevel = 4;
        window = sf::RenderWindow(sf::VideoMode({w, h}), title, sf::Style::Default, sf::State::Windowed, settings);
        window.setFramerateLimit(max_frame_rate);
        window.setMinimumSize(window.getSize());

        // CARICAMENTO FONT DIRETTAMENTE NELLO STATO:
        if (!font.openFromFile("Utilities/Font/PixelifySans-VariableFont_wght.ttf")) {
            std::cerr << "Errore nel caricamento del font!" << std::endl;
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

    // Offset
    float offset;
    if (isP1) {
        sf::FloatRect nameBounds = nameText.getLocalBounds();
        nameText.setOrigin({nameBounds.position.x + nameBounds.size.x / 2.0f, 
                            nameBounds.position.y + nameBounds.size.y / 2.0f});
        nameText.setPosition({110.f, 530.f});
    }
    else {
        sf::FloatRect nameBounds = nameText.getLocalBounds();
        nameText.setOrigin({nameBounds.position.x + nameBounds.size.x / 2.0f, 
                            nameBounds.position.y + nameBounds.size.y / 2.0f});
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
                        std::vector<sf::VertexArray>& lines, std::vector<sf::Text>& character_names, 
                        std::vector<sf::Text>& ball1_description, std::vector<sf::Text>& ball2_description) {
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



////////////
// Handle //
////////////

// Chiusura finestra
void handle (State& stato, const sf::Event::Closed& closeEvent) {
    stato.window.close();
}

// Pressione tasto
void handle (State& stato, const sf::Event::KeyPressed& keyEvent) {
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
    }
}

// Click mouse
void handle (State& stato, const sf::Event::MouseButtonPressed& mouseEvent, std::vector<sf::Text>& ball1_description, std::vector<sf::Text>& ball2_description) {
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
        if (!selectedSphere.imagePath.empty()) {
            if (stato.selected_balls == 0) {
                if (!stato.ball1.sphere_loader(radius_sphere_selected, selectedSphere, stato.font)) {
                    stato.window.close();
                }
                stato.ball1.ball.setPosition(sf::Vector2f(110.f, 640.f));
                update_sphere_data(ball1_description[0], ball1_description[1], ball1_description[2], selectedSphere, true);
                stato.selected_balls = 1;
            }
            else if (stato.selected_balls == 1) {
                if (!stato.ball2.sphere_loader(radius_sphere_selected, selectedSphere, stato.font)) {
                    stato.window.close();
                }
                stato.ball2.ball.setPosition(sf::Vector2f(890.f, 640.f));
                update_sphere_data(ball2_description[0], ball2_description[1], ball2_description[2], selectedSphere, false);
                stato.selected_balls = 2;
            }
        }
    }
}


////////////
// loader //
////////////


// Può ritornare errore in caso di file mancante
sf::Font font_loader (State& stato) {
    sf::Font font;
    if (!font.openFromFile("Utilities/Font/PixelifySans-VariableFont_wght.ttf")) {
        std::cerr << "Errore nel caricamento del font!" << std::endl;
        stato.window.close();
    }
    return font;
}

sf::Text title_loader (sf::Font& font) {
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

sf::Text subtitle_loader (sf::Font& font) {
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

std::vector<sf::RectangleShape> sphere_selects_loader() {
    // Variabili
    std::vector<sf::RectangleShape> sphere_selects;
    sphere_selects.reserve(6);
    sf::Vector2f select_size(120.f, 120.f);
    float thickness = 3.f;

    // Configurazione select
    auto setupSelect = [select_size, thickness] (sf::RectangleShape& sphere_select, sf::Vector2f pos) {
        sphere_select.setSize(select_size);
        sphere_select.setFillColor(sf::Color::Transparent);
        sphere_select.setOutlineColor(sf::Color::Black);
        sphere_select.setOutlineThickness(thickness);
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

// Può ritornare errore in caso di file mancante
std::vector<Sphere> spheres_loader(State& stato) {
    // Variabili
    std::vector<Sphere> spheres(6);

    // Configurazione sfere
    auto setupSphere = [&stato] (Sphere& sphere, SphereData dati, sf::Vector2f pos) {
        if (sphere.sphere_loader(radius_sphere_select, dati, stato.font)) {
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

std::vector<sf::VertexArray> lines_loader () {
    // Variabili
    std::vector<sf::VertexArray> lines;
    sf::VertexArray line1(sf::PrimitiveType::Lines, 2);
    sf::VertexArray line2(sf::PrimitiveType::Lines, 2);

    // Prima linea
    line1[0].position = sf::Vector2f(0.f, 440.f);
    line1[0].color = sf::Color::White;
    line1[1].position = sf::Vector2f(window_width, 440.f);
    line1[1].color = sf::Color::White;

    // Seconda linea
    line2[0].position = sf::Vector2f(500.f, 440.f);
    line2[0].color = sf::Color::White;
    line2[1].position = sf::Vector2f(500.f, 800.f);
    line2[1].color = sf::Color::White;

    // Push
    lines.push_back(line1);
    lines.push_back(line2);

    return lines;
}

std::vector<sf::Text> character_names_loader (sf::Font& font) {
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



//////////
// Loop //
//////////

int main () {
    State stato (window_width, window_height, window_title);

    // Font e testi
    sf::Text titleText = title_loader(stato.font);
    sf::Text subtitleText = subtitle_loader(stato.font);

    // Rettangoli
    std::vector<sf::RectangleShape> sphere_selects = sphere_selects_loader();

    // Sfere
    std::vector<Sphere> spheres = spheres_loader(stato);

    // Linee
    std::vector<sf::VertexArray> lines = lines_loader();

    // Nomi personaggi
    std::vector<sf::Text> character_names = character_names_loader(stato.font);

    // Descrizione sfere
    std::vector<sf::Text> ball1_description(3, sf::Text(stato.font));
    std::vector<sf::Text> ball2_description(3, sf::Text(stato.font));


    // Loop principale
    while (stato.window.isOpen()) {
        
        // Eventi
        stato.window.handleEvents (
            [&stato](const sf::Event::Closed& closeEvent) { 
                handle(stato, closeEvent); 
            },
            [&stato](const sf::Event::KeyPressed& keyEvent) { 
                handle(stato, keyEvent); 
            },
            [&stato, &ball1_description, &ball2_description](const sf::Event::MouseButtonPressed& mouseEvent) {
                handle(stato, mouseEvent, ball1_description, ball2_description);
            }
        );
        
        // Display
        if (stato.schermata == 0) {
            stato.window.clear(sf::Color::Black);
            drawTitle(stato, titleText, subtitleText);
        }
        else if (stato.schermata == 1) {
            stato.window.clear(sf::Color(0, 255, 255));
            drawSelectScreen(stato, sphere_selects, spheres ,lines, character_names, ball1_description, ball2_description);
        }
        stato.window.display();
    }

    return 0;
}
