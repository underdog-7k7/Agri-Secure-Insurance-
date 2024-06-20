// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract AgriSecure {
    uint16 public temp;
    uint16 public hum;
    uint16 public rainfall;
    uint16 public soilm;
    uint8 public currResult;
    bool public result;

    uint16 constant TEMP_THRESHOLD = 50; 
    uint16 constant HUM_THRESHOLD = 70; 
    uint16 constant RAINFALL_THRESHOLD = 1750;
    uint16 constant SOILM_THRESHOLD = 1500; 

    function setter(uint16 _temp, uint16 _hum, uint16 _rainfall, uint16 _soilm) public {
        temp = _temp;
        hum = _hum;
        rainfall = _rainfall;
        soilm = _soilm;
        
        if (_temp > TEMP_THRESHOLD || _hum > HUM_THRESHOLD || _rainfall < RAINFALL_THRESHOLD || _soilm < SOILM_THRESHOLD) {
            result = true;
            currResult = 1;
        }
        else{
            currResult = 0;
        }
    }

    function getter() public view returns(uint16, uint16, uint16, uint16, uint8, bool) {
        return (temp, hum, rainfall, soilm, currResult,result);
    }
}
