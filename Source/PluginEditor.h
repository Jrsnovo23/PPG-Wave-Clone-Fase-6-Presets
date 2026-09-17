#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <array>
#include "PluginProcessor.h"

/**
    Editor de la Fase 6: agrega la barra de presets (fábrica + usuario)
    sobre la base de la Fase 5. Sigue siendo una interfaz genérica de
    JUCE, no la estética final — eso es la Fase 7.

    Alcance de esta barra provisional: selector + anterior/siguiente/random
    + favorito + guardar/eliminar. NO incluye el buscador de texto ni el
    filtro por categoría en vivo — PresetManager::findMatching() ya los
    soporta a nivel de datos; conectarlos a controles de texto/combo reales
    es trabajo de interfaz, no de lógica, y queda para la Fase 7.

    Notas de alcance heredadas de fases previas:
    - Sync (on/off) y Retrigger de cada LFO no tienen control dedicado aquí.
    - Delay Sync (on/off) y Sync Division tampoco tienen control dedicado.
    Todos automatizables desde Ableton Live.
*/
class PPGWaveCloneAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit PPGWaveCloneAudioProcessorEditor (PPGWaveCloneAudioProcessor&);
    ~PPGWaveCloneAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    struct OscillatorControls
    {
        juce::Label title;
        juce::ComboBox wavetableBox;
        juce::Slider positionSlider, octaveSlider, coarseSlider, fineSlider, levelSlider;
        juce::Label positionLabel, octaveLabel, coarseLabel, fineLabel, levelLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> wavetableAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            positionAttachment, octaveAttachment, coarseAttachment, fineAttachment, levelAttachment;
    };

    struct FilterControls
    {
        juce::Label title;
        juce::ComboBox typeBox;
        juce::Slider cutoffSlider, resonanceSlider, keyTrackSlider, envAmountSlider;
        juce::Label cutoffLabel, resonanceLabel, keyTrackLabel, envAmountLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> typeAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            cutoffAttachment, resonanceAttachment, keyTrackAttachment, envAmountAttachment;
    };

    struct EnvelopeControls
    {
        juce::Label title;
        juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider;
        juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            attackAttachment, decayAttachment, sustainAttachment, releaseAttachment;
    };

    struct LFOControls
    {
        juce::Label title;
        juce::ComboBox waveformBox;
        juce::Slider rateSlider, phaseSlider, depthSlider;
        juce::Label rateLabel, phaseLabel, depthLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveformAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            rateAttachment, phaseAttachment, depthAttachment;
    };

    struct ModSlotControls
    {
        juce::ComboBox sourceBox, destinationBox;
        juce::Slider amountSlider;

        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> sourceAttachment, destinationAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> amountAttachment;
    };

    struct EffectControls
    {
        juce::Label title;
        juce::ToggleButton enableButton;
        juce::Slider knob1Slider, knob2Slider, knob3Slider;
        juce::Label knob1Label, knob2Label, knob3Label;

        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enableAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            knob1Attachment, knob2Attachment, knob3Attachment;
    };

    void setupOscillatorControls (OscillatorControls& controls, int oscNumber);
    void setupFilterControls();
    void setupEnvelopeControls (EnvelopeControls& controls, int envNumber, const juce::String& titleText);
    void setupLFOControls (LFOControls& controls, int lfoNumber);
    void setupModSlotControls (ModSlotControls& controls, int slotNumber);
    void setupEffectControls (EffectControls& controls, const juce::String& titleText,
                               const juce::String& enableParamID,
                               const juce::String& knob1ParamID, const juce::String& knob1Text,
                               const juce::String& knob2ParamID, const juce::String& knob2Text,
                               const juce::String& knob3ParamID, const juce::String& knob3Text);

    void layoutOscillatorControls (OscillatorControls& controls, juce::Rectangle<int> area);
    void layoutFilterControls (juce::Rectangle<int> area);
    void layoutEnvelopeControls (EnvelopeControls& controls, juce::Rectangle<int> area);
    void layoutLFOControls (LFOControls& controls, juce::Rectangle<int> area);
    void layoutModSlotControls (ModSlotControls& controls, juce::Rectangle<int> area);
    void layoutEffectControls (EffectControls& controls, juce::Rectangle<int> area);

    void setupPresetBar();
    void layoutPresetBar (juce::Rectangle<int> area);
    void refreshPresetComboBox();
    void updateFavoriteButtonState();
    void onSaveButtonClicked();

    PPGWaveCloneAudioProcessor& processorRef;

    juce::Label titleLabel;
    juce::Slider masterVolumeSlider;
    juce::Label  masterVolumeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterVolumeAttachment;

    OscillatorControls osc1Controls, osc2Controls;
    FilterControls filterControls;
    EnvelopeControls env1Controls, env2Controls;
    LFOControls lfo1Controls, lfo2Controls;
    juce::Label modMatrixTitle;
    std::array<ModSlotControls, 4> modSlotControls;
    EffectControls driveControls, chorusControls, delayControls, reverbControls;

    juce::ComboBox presetComboBox;
    juce::TextButton presetPrevButton { "<" }, presetNextButton { ">" }, presetRandomButton { "Random" },
                      presetFavoriteButton { "Fav" }, presetSaveButton { "Save" }, presetDeleteButton { "Delete" };
    std::unique_ptr<juce::AlertWindow> saveNameWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PPGWaveCloneAudioProcessorEditor)
};
