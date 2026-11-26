#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class GeothermalAsset {
protected:
    string wellID;
    bool isActive;

public:
    static int activeWells;

    GeothermalAsset(string id) : wellID(id), isActive(true) {
        activeWells++;
    }

    virtual ~GeothermalAsset() {
        activeWells--;
    }

    virtual void runTelemetryCycle() = 0;

    friend ostream& operator<<(ostream& out, const GeothermalAsset& asset) {
        out << "[Well ID: " << asset.wellID << " | Status: " << (asset.isActive ? "ONLINE" : "OFFLINE") << "]";
        return out;
    }
};

int GeothermalAsset::activeWells = 0;

class ThermalExtractionModule : virtual public GeothermalAsset {
protected:
    double brineTempCelsius;

public:
    ThermalExtractionModule(string id, double temp) : GeothermalAsset(id), brineTempCelsius(temp) {}

    void monitorBrine() {
        cout << "Thermal extraction [" << wellID << "] brine temp: " << brineTempCelsius << " C." << endl;
    }
};

class SeismicSensorModule : virtual public GeothermalAsset {
protected:
    double tremorMagnitude;

public:
    SeismicSensorModule(string id, double magnitude) : GeothermalAsset(id), tremorMagnitude(magnitude) {}

    void monitorSeismicActivity() {
        cout << "Seismic array [" << wellID << "] micro-tremor reading: " << tremorMagnitude << " Mw." << endl;
    }
};

class DeepGeothermalStation : public ThermalExtractionModule, public SeismicSensorModule {
private:
    double netOutputMW;

public:
    DeepGeothermalStation(string id, double temp, double magnitude, double output)
        : GeothermalAsset(id),
          ThermalExtractionModule(id, temp),
          SeismicSensorModule(id, magnitude),
          netOutputMW(output) {}

    ~DeepGeothermalStation() {
        cout << "Station " << wellID << " injection valves sealed and offline." << endl;
    }

    void runTelemetryCycle() override {
        cout << "\n--- Geothermal Station Telemetry: " << *this << " ---" << endl;
        monitorBrine();
        monitorSeismicActivity();
        cout << "Net Electrical Output: " << fixed << setprecision(2) << netOutputMW << " MW" << endl;
    }
};

int main() {
    const int GRID_SIZE = 2;
    GeothermalAsset* grid[GRID_SIZE];

    grid[0] = new DeepGeothermalStation("GEO-ICELAND-01", 315.4, 1.2, 84.50);
    grid[1] = new DeepGeothermalStation("GEO-RIFT-09", 289.0, 0.8, 62.25);

    for (int i = 0; i < GRID_SIZE; i++) {
        grid[i]->runTelemetryCycle();
    }

    cout << "\nActive Subterranean Wells: " << GeothermalAsset::activeWells << endl;

    for (int i = 0; i < GRID_SIZE; i++) {
        delete grid[i];
    }

    cout << "Active Wells After Grid Shutdown: " << GeothermalAsset::activeWells << endl;

    return 0;
}
