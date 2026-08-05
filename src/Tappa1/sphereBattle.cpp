#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <string>
#include <iostream>

////////////////////////
// Variabili Iniziali //
////////////////////////

const char* window_title = "Sphere Battle";
const unsigned window_width = 1000;
const unsigned window_height = 800;
const float max_frame_rate = 60;
int schermata = 0;


//////////
// Draw //
//////////

// Schermata del titolo
void drawTitle (sf::RenderWindow& window, const sf::Text& titleText, const sf::Text& subtitleText) {
    window.draw(titleText);
    window.draw(subtitleText);
}

// Character Select
void drawSelectScreen (sf::RenderWindow& window, std::vector<sf::RectangleShape>& sphere_selects, std::vector<sf::VertexArray>& lines, std::vector<sf::Text>& character_names) {
    for (const auto& sphere_select : sphere_selects) {
        window.draw(sphere_select);
    }
    for (const auto& line : lines) {
        window.draw(line);
    }
    for (const auto& character_name : character_names) {
        window.draw(character_name);
    }
}


////////////
// Handle //
////////////

void handle_close (sf::RenderWindow& window) {
    window.close();
}

void handle_key (sf::RenderWindow& window) {
    schermata = 1;
}



////////////////
// Ausiliarie //
////////////////

sf::Font font_loader (sf::RenderWindow& window) {
    sf::Font font;
    if (!font.openFromFile("Utilities/Font/PixelifySans-VariableFont_wght.ttf")) {
        std::cerr << "Errore nel caricamento del font!" << std::endl;
        window.close();
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

std::vector<sf::RectangleShape> sphere_select_loader() {
    // Variabili
    std::vector<sf::RectangleShape> sphere_selects;
    sphere_selects.reserve(6);
    sf::Vector2f select_size(120.f, 120.f);
    float thickness = 3.f;

    // Configurazione select
    auto setupSelect = [select_size, thickness] (sf::RectangleShape& sphere_select, sf::Color color, sf::Vector2f pos) {
        sphere_select.setSize(select_size);
        sphere_select.setFillColor(sf::Color::Transparent);
        sphere_select.setOutlineColor(color);
        sphere_select.setOutlineThickness(thickness);
        sphere_select.setPosition(pos);
    };

    // Inizializzazione e modifica valori
    sf::RectangleShape sphere_select1, sphere_select2, sphere_select3;
    sf::RectangleShape sphere_select4, sphere_select5, sphere_select6;

    setupSelect(sphere_select1, sf::Color(255, 176, 0), sf::Vector2f(180.f, 40.f));
    setupSelect(sphere_select2, sf::Color(247, 255, 0), sf::Vector2f(440.f, 40.f));
    setupSelect(sphere_select3, sf::Color(0, 255, 27), sf::Vector2f(700.f, 40.f));
    setupSelect(sphere_select4, sf::Color(255, 0, 0), sf::Vector2f(180.f, 240.f));
    setupSelect(sphere_select5, sf::Color(67, 0, 255), sf::Vector2f(440.f, 240.f));
    setupSelect(sphere_select6, sf::Color(252, 0, 255), sf::Vector2f(700.f, 240.f));

    // Push 
    sphere_selects.push_back(sphere_select1);
    sphere_selects.push_back(sphere_select2);
    sphere_selects.push_back(sphere_select3);
    sphere_selects.push_back(sphere_select4);
    sphere_selects.push_back(sphere_select5);
    sphere_selects.push_back(sphere_select6);

    return sphere_selects;
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
    sf::Text character_name1 (font, "Sphere 1", font_size);
    sf::Text character_name2 (font, "Sphere 2", font_size);
    sf::Text character_name3 (font, "Sphere 3", font_size);
    sf::Text character_name4 (font, "Sphere 4", font_size);
    sf::Text character_name5 (font, "Sphere 5", font_size);
    sf::Text character_name6 (font, "Sphere 6", font_size);

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
    sf::RenderWindow window (sf::VideoMode ({window_width, window_height}), window_title, sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(max_frame_rate);
    window.setMinimumSize(window.getSize());

    // Font e testi
    sf::Font font = font_loader(window);
    sf::Text titleText = title_loader(font);
    sf::Text subtitleText = subtitle_loader(font);

    // Rettangoli
    std::vector<sf::RectangleShape> sphere_selects = sphere_select_loader();

    // Linee
    std::vector<sf::VertexArray> lines = lines_loader();

    // Nomi personaggi
    std::vector<sf::Text> character_names = character_names_loader(font);


    // Loop principale
    while (window.isOpen()) {
        

        // Eventi
        window.handleEvents (
            [&window](const sf::Event::Closed&) { 
                handle_close(window); 
            },
            [&window](const sf::Event::KeyPressed&) { 
                handle_key(window); 
            }
        );
        
        // Display
        if (schermata == 0) {
            window.clear(sf::Color::Black);
            drawTitle(window, titleText, subtitleText);
        }
        else if (schermata == 1) {
            window.clear(sf::Color(0, 255, 255));
            drawSelectScreen(window, sphere_selects, lines, character_names);
        }
        window.display();
    }

    return 0;
}
