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
    
    // Menu initialization
    mFilterBox = new ComboBox;
    
    mFilterBox->addItem("Hamming LPF", 1);
    mFilterBox->addItem("Blackman Harris LPF", 2);
    mFilterBox->addItem("Kaiser LPF", 3);
    mFilterBox->addItem("Transition Method LPF", 4);
    mFilterBox->setJustificationType (Justification::centred);
    addAndMakeVisible (mFilterBox);
    
    mFilterType = new AudioProcessorValueTreeState::ComboBoxAttachment (p.getValueTreeState(), "filterType", *mFilterBox);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (720, 480);
}

WholeBunchOfFiltersAudioProcessorEditor::~WholeBunchOfFiltersAudioProcessorEditor()
{
}

//==============================================================================
void WholeBunchOfFiltersAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::black);
    g.setFont (75.0f);
    g.drawFittedText ("SO MANY FILTERS", getLocalBounds(), Justification::centred, 1);
}

void WholeBunchOfFiltersAudioProcessorEditor::resized()
{
    const int removal = 40;
    
    Rectangle<int> componentSpace = getLocalBounds().reduced(removal);
    
    // Going to be implemented later
    float width  = componentSpace.getWidth();
    float height = componentSpace.getHeight();
    
    const int dialSize = 100;
    
    mCutoffSlider->setBounds (removal, 10, dialSize, dialSize);
    mFilterBox->setBounds (140, 40, 520, 30);
    
}
