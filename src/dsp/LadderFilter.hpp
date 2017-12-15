#pragma once

#include "DSPEffect.hpp"
#include "engine.hpp"

namespace rack {

    struct LadderFilter : DSPEffect {

        Input audioIn;
        Param cutoffParam, resonanceParam, driveParam;
        Output lpOut, bpOut, hpOut;


        void invalidate() override {
            DSPEffect::invalidate();
        }


        double process() override {
            return DSPEffect::process();
        }


        const Input &getAudioIn() const {
            return audioIn;
        }


        void setAudioIn(const Input &audioIn) {
            LadderFilter::audioIn = audioIn;
        }


        const Param &getCutoffParam() const {
            return cutoffParam;
        }


        void setCutoffParam(const Param &cutoffParam) {
            LadderFilter::cutoffParam = cutoffParam;
        }


        const Param &getResonanceParam() const {
            return resonanceParam;
        }


        void setResonanceParam(const Param &resonanceParam) {
            LadderFilter::resonanceParam = resonanceParam;
        }


        const Param &getDriveParam() const {
            return driveParam;
        }


        void setDriveParam(const Param &driveParam) {
            LadderFilter::driveParam = driveParam;
        }


        const Output &getLpOut() const {
            return lpOut;
        }


        void setLpOut(const Output &lpOut) {
            LadderFilter::lpOut = lpOut;
        }


        const Output &getBpOut() const {
            return bpOut;
        }


        void setBpOut(const Output &bpOut) {
            LadderFilter::bpOut = bpOut;
        }


        const Output &getHpOut() const {
            return hpOut;
        }


        void setHpOut(const Output &hpOut) {
            LadderFilter::hpOut = hpOut;
        }

    };
}