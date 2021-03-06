widgetViewModelCtor =

   /**
    * Create a Shutter ViewModel
    * @constructor
    */
      function shutterViewModel() {
          var self = this;
          //observable data
          this.command = ko.observable(1);
          this.kind = ko.observable("simple");
          this.invert = ko.observable(false);
          this.icon = ko.observable("");
          this.readonly = ko.observable(true);

          // default size
          this.WidgetHeight = 70;
          this.WidgetWidth = 95;

          this.commandClick = function (newState) {

              var self = this;
              if(self.readonly() !== true) {
                 if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
                     var cmd = null;

                     cmd = newState;
                     KeywordManager.sendCommand(this.widget.configuration.device.keywordId, cmd.toString());
                 }
              }
          };

          self.shutterIcon = ko.computed(function () {
              
              var displayValue = self.command();
              
              if (self.invert()) {
                  displayValue = !displayValue;
              }
              
              if (self.kind() !== "verticalInverter")
              {
                 if (displayValue)
                     return "widgets/shutter/icons/" + self.kind() + "-close.png";
                 else
                     return "widgets/shutter/icons/" + self.kind() + "-open.png";
              }
              else 
                 return "widgets/shutter/icons/window-open.png"; // default value
          });

          /**
           * Initialization method
           * @param widget widget class object
           */
          this.initialize = function () {
              //we configure the toolbar
              this.widgetApi.toolbar({
                  activated: true,
                  displayTitle: true,
                  batteryItem: true
              });
          };

          this.shutterClick = function () {
              var self = this;
              
              if(self.readonly() !== true) {
                 if (self.command() === 0)
                     self.command(1);
                 else
                     self.command(0);

                 //Send the command
                 this.commandClick(self.command());
              }
          }

          this.configurationChanged = function () {
              var self = this;

              if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
                  return;

              if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.device))) {
                  self.widgetApi.registerKeywordAcquisitions(this.widget.configuration.device.keywordId);

                  // Get the capacity of the keyword
                  var deffered = KeywordManager.get(this.widget.configuration.device.keywordId)
                 .done(function (keyword) {
                     if (keyword.accessMode === "GetSet")
                         self.readonly(false);
                     else
                         self.readonly(true);
                 });
              }

              if (!isNullOrUndefined(this.widget.configuration.kind)) {
                  this.kind(this.widget.configuration.kind.activeSection);
              }

              if (!isNullOrUndefined(this.widget.configuration.invert)) {
                  this.invert(parseBool(this.widget.configuration.invert));
              }
          };

          /**
          * New acquisition handler
          * @param keywordId keywordId on which new acquisition was received
          * @param data Acquisition data
          */
          this.onNewAcquisition = function (keywordId, data) {
              var self = this;

              if ((this.widget.configuration != undefined) && (this.widget.configuration.device != undefined)) {

                  if (keywordId === this.widget.configuration.device.keywordId) {

                      // Adapt for dimmable or switch capacities
                      if (parseInt(data.value) !== 0)
                          self.command(1);
                      else
                          self.command(0);
                  }
              }
          };
      };