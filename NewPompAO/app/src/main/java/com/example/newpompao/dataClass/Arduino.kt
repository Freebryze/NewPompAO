package com.example.newpompao.dataClass

public class Arduino {

    var LastUpdateId: Int
    var isPumpActivated : Boolean = false
    var UV: Int
    var date: String
    var groundHumidity: Int
    var humidity: Int
    var temperature: Int

    constructor(LastUpdateId : Int, isPumpActivated : Boolean, UV: Int, date: String, groundHumidity: Int, humidity: Int, temperature: Int){
        this.LastUpdateId = LastUpdateId
        this.isPumpActivated = isPumpActivated
        this.UV = UV
        this.date = date
        this.groundHumidity = groundHumidity
        this.humidity = humidity
        this.temperature = temperature
    }

    constructor(){
        this.LastUpdateId = -1
        this.isPumpActivated = false
        this.UV = -1
        this.date = ""
        this.groundHumidity = -1
        this.humidity = -1
        this.temperature = -1
    }

}

