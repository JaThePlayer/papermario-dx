#pragma once

enum WeatherTypes {
    WEATHER_TutanPoisonFog = 0,
};

// If the weather was delayed init, initializes it.
// Args: WeatherTypes type
extern API_CALLABLE(A(EnableWeatherByType));
