# Use Open-Meteo for Ambient Status weather

Blip uses Open-Meteo’s non-commercial, no-key service behind a replaceable weather-provider interface. The user manually selects a city, whose coordinates and display name are stored without requesting macOS Location permission by default. Blip fetches only current conditions and optional daily high/low, refreshes no more than every 15 minutes, caches successful results, marks stale data with its age, supports configurable units, and shows required Open-Meteo attribution in Control Center. Weather failure never blocks core device operation.
