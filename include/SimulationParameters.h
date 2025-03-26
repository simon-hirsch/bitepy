#ifndef SIMULATIONPARAMETERS_H
#define SIMULATIONPARAMETERS_H

#include <string>

class SimulationParameters {
public:
    SimulationParameters();
    ~SimulationParameters();

    // Getters
    double getStorageMaxPy() const;
    int    getStartMonthPy() const;
    int    getEndMonthPy() const;
    int    getStartDayPy() const;
    int    getStartHourPy() const;
    int    getEndHourPy() const;
    int    getEndDayPy() const;
    int    getStartYearPy() const;
    int    getEndYearPy() const;
    double getDpFreqPy() const;
    double getLossInPy() const;
    double getLossOutPy() const;
    double getLinDegCostPy() const;
    std::string getRunTypePy() const;
    std::string getTempDirPy() const;
    std::string getResultsDirPy() const;
    int    getNumStorStatesPy() const;
    int    getStoRoundDecPy() const;
    int    getPingDelayPy() const;
    int    getFixedSolveTimePy() const;
    bool   getCheckProfitPy() const;
    bool   getCheckLOExecPy() const;
    bool   getUseSlidingPy() const;
    std::string getCmaesParamPathPy() const;
    double getWithdrawMaxPy() const;
    double getInjectMaxPy() const;
    double getTradingFeePy() const;
    bool   getUseGurobiPy() const;
    int    getGurobiTimeoutPy() const;
    int    getForeHorizonStartPy() const;
    int    getForeHorizonEndPy() const;
    double getBidAskPenaltyPy() const;

    // Setters
    void setStorageMaxPy(double storM);
    void setStartMonthPy(int startM);
    void setEndMonthPy(int endM);
    void setStartDayPy(int startD);
    void setStartHourPy(int startH);
    void setEndHourPy(int endH);
    void setEndDayPy(int endD);
    void setStartYearPy(int startY);
    void setEndYearPy(int endY);
    void setDpFreqPy(double dpF);
    void setLossInPy(double lossI);
    void setLossOutPy(double lossO);
    void setLinDegCostPy(double linDC);
    void setRunTypePy(const std::string& runT);
    void setTempDirPy(const std::string& tempD);
    void setResultsDirPy(const std::string& resultsD);
    void setNumStorStatesPy(int numSS);
    void setStoRoundDecPy(int stoRD);
    void setPingDelayPy(int pingD);
    void setFixedSolveTimePy(int fixedST);
    void setCheckProfitPy(bool checkP);
    void setCheckLOExecPy(bool checkLOE);
    void setUseSlidingPy(bool useS);
    void setCmaesParamPathPy(const std::string& cmaesPP);
    void setWithdrawMaxPy(double withM);
    void setInjectMaxPy(double injM);
    void setTradingFeePy(double tradF);
    void setUseGurobiPy(bool useG);
    void setGurobiTimeoutPy(int gurobiT);
    void setForeHorizonStartPy(int foreHorizonS);
    void setForeHorizonEndPy(int foreHorizonE);
    void setBidAskPenaltyPy(double bidAskP);

    void printParameters() const;

private:
    // Use the Pimpl idiom to hide the internal data members and helper functions.
    struct Impl;
    Impl* impl;
};

#endif // SIMULATIONPARAMETERS_H