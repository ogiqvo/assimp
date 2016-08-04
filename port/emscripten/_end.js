    if (typeof define === "function" && define.amd) {
        define(assimp);
    } else if (typeof module === "object" && module.exports) {
        module.exports = assimp;
    } else {
        this.assimp = assimp;
    }
}).apply(this);
