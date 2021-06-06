package com.example.newpompao.ui.home

import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import com.example.newpompao.R
import com.example.newpompao.dataClass.Arduino
import com.example.newpompao.dataClass.Farm
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.ValueEventListener

class HomeFragment : Fragment() {

    private lateinit var homeViewModel: HomeViewModel
    private lateinit var myEventListener: ValueEventListener

    private lateinit var humidityTextView: TextView
    private lateinit var UVTextView: TextView
    private lateinit var groundHumidityTextView: TextView
    private lateinit var temperatureTextView: TextView
    private lateinit var dateLastUpdateTextView: TextView

    override fun onCreateView(
            inflater: LayoutInflater,
            container: ViewGroup?,
            savedInstanceState: Bundle?
    ): View? {
        homeViewModel =
                ViewModelProvider(this).get(HomeViewModel::class.java)
        val root = inflater.inflate(R.layout.fragment_home, container, false)

        return root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        humidityTextView = view.findViewById(R.id.humidityValueTextView)
        UVTextView = view.findViewById(R.id.UVValueTextView)
        groundHumidityTextView = view.findViewById(R.id.groundHumidityValueTextView)
        temperatureTextView = view.findViewById(R.id.temperatureValueTextView)
        dateLastUpdateTextView = view.findViewById(R.id.dateLastUpdate)

        myEventListener = object : ValueEventListener {
            override fun onCancelled(error: DatabaseError) {
                TODO("Not yet implemented")
            }

            override fun onDataChange(snapshot: DataSnapshot) {
                var myArduino: Arduino
                myArduino = snapshot.getValue(Arduino::class.java)!!

                humidityTextView.text = myArduino.humidity.toString()
                UVTextView.text = myArduino.UV.toString()
                groundHumidityTextView.text = myArduino.groundHumidity.toString()
                temperatureTextView.text = myArduino.temperature.toString()
                dateLastUpdateTextView.text = "Date de la dernière mise à jour : " + myArduino.date

            }
        }

        homeViewModel.myRef.addValueEventListener(myEventListener)

    }
}