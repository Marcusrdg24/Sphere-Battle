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

void drawTitle (sf::RenderWindow& window, const sf::Text& titleText, const sf::Text& subtitleText) {
    window.draw(titleText);
    window.draw(subtitleText);
}

void drawSelectScreen (sf::RenderWindow& window, std::vector<sf::RectangleShape>& sphere_selects) {
    for (const auto& sphere_select : sphere_selects) {
        window.draw(sphere_select);
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
    if (!font.openFromFile("src/Utilities/GeistPixel-Regular-VariableFont_ELSH.ttf")) {
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
    sf::Text subtitleText(font, "press any key", 30); // Dimensione 22px
    subtitleText.setFillColor(sf::Color(180, 180, 180)); // Grigio chiaro

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
    setupSelect(sphere_select4, sf::Color(0, 255, 255), sf::Vector2f(180.f, 240.f));
    setupSelect(sphere_select5, sf::Color(67, 0, 255), sf::Vector2f(440.f, 240.f));
    setupSelect(sphere_select6, sf::Color(252, 0, 255), sf::Vector2f(700.f, 240.f));

    // push 
    sphere_selects.push_back(sphere_select1);
    sphere_selects.push_back(sphere_select2);
    sphere_selects.push_back(sphere_select3);
    sphere_selects.push_back(sphere_select4);
    sphere_selects.push_back(sphere_select5);
    sphere_selects.push_back(sphere_select6);

    return sphere_selects;
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
            window.clear(sf::Color::White);
            drawSelectScreen(window, sphere_selects);
        }
        window.display();
    }

    return 0;
}
