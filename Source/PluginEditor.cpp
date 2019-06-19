/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WholeBunchOfFiltersAudioProcessorEditor::WholeBunchOfFiltersAudioProcessorEditor (WholeBunchOfFiltersAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    mCutoffSlider = new Slider (Slider::RotaryVerticalDrag, Slider::TextBoxBelow);
    addAndMakeVisible (mCutoffSlider);
    mCutoffAttachment = new AudioProcessorValueTreeState::SliderAttachment (p.getValueTreeState(), "cutoff", *mCutoffSlider);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

WholeBunchOfFiltersAudioProcessorEditor::~WholeBunchOfFiltersAudioProcessorEditor()
{
}

//==============================================================================
void WholeBunchOfFiltersAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void WholeBunchOfFiltersAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
