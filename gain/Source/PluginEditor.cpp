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
    gainVolume.setRange(-20.0, 20.0, 0.01);
    gainVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    gainVolume.setPopupDisplayEnabled(true, false, this);
    gainVolume.setTextValueSuffix(" dB");
    gainVolume.setValue(0.0);

    // this function adds the slider to the editor
    addAndMakeVisible(&gainVolume);
    // add the listener to the slider
    gainVolume.addListener(this);

    // Configure gainTextEditor
    gainTextEditor.setText("0.00");
    gainTextEditor.onTextChange = [this] {
        float newTextValue = gainTextEditor.getText().getFloatValue();

        // Update the slider value
        gainVolume.setValue(newTextValue);

        // Use the public method setGain to update the gain parameter in the processor
        audioProcessor.setGain(newTextValue);

        // There's no need to directly set a gainVal member in the processor
        // since the gain is managed by the setGain method and gain parameter
     };
    addAndMakeVisible(gainTextEditor);

    debug.setText("debug", juce::dontSendNotification);
    addAndMakeVisible(debug);
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
    gainTextEditor.setBounds(labelX, labelY, labelWidth, labelHeight);

    debug.setBounds(150, 150, 50, 50);
}

void GainAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    if (slider == &gainVolume) {
        float sliderValue = gainVolume.getValue();

        // Convertir la valeur du Slider en une valeur normalisée pour le paramètre
        float normalizedValue = (sliderValue - gainVolume.getMinimum()) / (gainVolume.getMaximum() - gainVolume.getMinimum());

        // Mettre à jour le paramètre
        audioProcessor.getParameters()[0]->setValueNotifyingHost(normalizedValue);

        debug.setText(juce::String(audioProcessor.getGain()), juce::dontSendNotification);
    }
}
