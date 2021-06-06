package com.example.newpompao.ui.home

import androidx.lifecycle.ViewModel
import com.google.firebase.database.FirebaseDatabase

class HomeViewModel : ViewModel() {

    var database = FirebaseDatabase.getInstance()
    var myRef = database.getReference("arduino0")

}