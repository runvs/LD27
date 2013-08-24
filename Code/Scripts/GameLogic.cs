using System;
using Gibbo.Library;
using Microsoft.Xna.Framework;

namespace Scripts
{
    public class GameLogic : ObjectComponent
    {
        /// <summary>
        /// The Player's Time which is left for gaming
        /// </summary>
        public float RemainingTimeInMilliSeconds
        {
            get;
            private set;
        }

        /// <summary>
        /// The Time, the player starts with
        /// </summary>
        public float StartTimeInMilliSeconds
        {
            get;
            set;
        }


        GameObject _testObject;

        /// <summary>
        /// Initialize this instance.
        /// </summary>
        public override void Initialize()
        {
            base.Initialize();

            // TODO: initialize your component members
            RemainingTimeInMilliSeconds = StartTimeInMilliSeconds;
            _testObject = new GameObject();
   

        }

        /// <summary>
        /// Component Update 
        /// </summary>
        /// <param name="gameTime">Game time.</param>
        public override void Update(Microsoft.Xna.Framework.GameTime gameTime)
        {
            base.Update(gameTime);

            // TODO: update your component logic
            RemainingTimeInMilliSeconds -= gameTime.ElapsedGameTime.Milliseconds;

            // Console.WriteLine(RemainingTimeInMilliSeconds);
        }

        /// <summary>
        /// Component Draw
        /// </summary>
        /// <param name="gameTime">Game time.</param>
        /// <param name="spriteBatch">Sprite batch.</param>
        public override void Draw(Microsoft.Xna.Framework.GameTime gameTime, Microsoft.Xna.Framework.Graphics.SpriteBatch spriteBatch)
        {
            base.Draw(gameTime, spriteBatch);

            // TODO: draw your component members

            _testObject.Draw(gameTime, spriteBatch);

        }
    }
}
