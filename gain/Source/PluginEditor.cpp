/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainAudioProcessorEditor::GainAudioProcessorEditor (GainAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 200);

    // these define the parameters of our slider object
    gainVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    gainVolume.setRange(-20, 20.0, 0.01);
    gainVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    gainVolume.setPopupDisplayEnabled(true, false, this);
    gainVolume.setTextValueSuffix(" dB");
    gainVolume.setValue(0.0);

    // this function adds the slider to the editor
    addAndMakeVisible(&gainVolume);
    // add the listener to the slider
    gainVolume.addListener(this);


    // Configure gainLabel
    gainLabel.setText("Gain: 0.00 dB", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainLabel);
}

GainAudioProcessorEditor::~GainAudioProcessorEditor()
{
}

//==============================================================================
void GainAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText("Gain in dB", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void GainAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // sets the position and size of the slider with arguments (x, y, width, height)
    gainVolume.setBounds(40, 30, 20, getHeight() - 60);    
    // Place the label to the right of the slider. Adjust the x, y, width, and height as needed
    int labelX = gainVolume.getRight() + 10; // 10 pixels to the right of the slider
    int labelY = gainVolume.getY(); // same vertical position as the slider
    int labelWidth = 100;
    int labelHeight = 20;
    gainLabel.setBounds(labelX, labelY, labelWidth, labelHeight);
}

void GainAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.gainVal = gainVolume.getValue();
    gainLabel.setText("Gain: " + juce::String(audioProcessor.gainVal) + " dB", juce::dontSendNotification);
}