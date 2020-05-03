#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../include/ResourcesManager.hpp"

template<>
// If Resources used with images, declaring the map containing textures
std::unordered_map<std::string_view, sf::Texture> ResourcesManager<sf::Texture>::resources{};

template<>
// If Resources used with sound, declaring the map containing sound buffer
std::unordered_map<std::string_view, sf::SoundBuffer> ResourcesManager<sf::SoundBuffer>::resources{};
